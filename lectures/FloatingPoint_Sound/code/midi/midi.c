#include "midi.h"
#include "gpio.h"
#include "timer.h"
#include "gpio_extra.h"
#include "printf.h"

#define MIDI_PIN GPIO_PB4
// works:
#define BAUD     33500

// actual should be:
// #define BAUD     31250
#define DELAY    (1000000 / BAUD)

static unsigned channel = 0;

void midi_init() {
  gpio_set_output(MIDI_PIN);
  gpio_set_pulldown(MIDI_PIN);
  midi_reset();
}

// Need inversion: a logical 1 is a high signal
void send_bit(uint8_t val) {
  gpio_write(MIDI_PIN, val);
  timer_delay_us(DELAY);
}

void send_byte(uint8_t val) {
  unsigned i;
  send_bit(0); // start bit
  for (i = 0; i < 8; i++) { // data bits, LSB first
    send_bit((val >> i) & 1);
  }
  send_bit(1); // stop bit
}

void midi_reset() {
    uint8_t cmd[] = {0xff};
    midi_send(cmd, 1);
}

void midi_send(uint8_t* cmd, unsigned len) {
  unsigned i;
  for (i = 0; i < len; i++) {
    send_byte(cmd[i]);
  }
}

void midi_note_on(uint8_t key, uint8_t velocity) {
  uint8_t command[3];
  // Note on is 1001cccc where cccc is the channel
  command[0] = 0x90 | (channel & 0xf);
  command[1] = key;
  command[2] = velocity;
  midi_send(command, 3);
}

void midi_note_off(uint8_t key, uint8_t velocity) {
  uint8_t command[3];
  // Note off is 1000cccc where cccc is the channel
  command[0] = 0x80 | (channel & 0xf);
  command[1] = key;
  command[2] = velocity;
  midi_send(command, 3);
}

void midi_test() {
  unsigned command = 0xaaaa5555;
  midi_send((uint8_t*)&command, 4);
}

void midi_set_channel(unsigned ch) {
  channel = ch;
}

unsigned midi_channel() {
  return channel;
}

void midi_print(uint8_t* cmd, unsigned len) {
    for (unsigned int i = 0; i < len; i++) {
        printf("%x, ", cmd[i]);
    }
    printf("\n");
}

void play_midi(uint8_t *data, int tempo) {
   // read headers
   // first four bytes should be "MThd"
   if (data[0] != 'M' || data[1] != 'T' || data[2] != 'h' || data[3] != 'd') {
       printf("Not recognized as MIDI data (no 'MThd' at start). :(\n");
       return;
   }
   // the next four bytes is the big-endian number 6
   if (data[4] != 0 || data[5] != 0 || data[6] != 0 || data[7] != 6) {
       printf("Not recognized as MIDI data (magic number 6 missing). :(\n");
       return;
   }

   // there are three 16-bit words to follow
   // 1. format (we can only handle type 0)
   if (data[8] != 0 || data[9] != 0) {
       printf("Not a type-0 MIDI file. There are converstion tools available online.\n");
       return;
   }

   // 2. number of tracks (must be 1)
   if (data[10] != 0 || data[11] != 1) {
       printf("The file does not seem to have only one track.\n");
       return;
   }

   // 3. division (for timing -- we will ignore the next two bytes, data[12,13]
    
   // The next four bytes should be 'MTrk' 
   if (data[14] != 'M' || data[15] != 'T' || data[16] != 'r' || data[17] != 'k') {
       printf("No track found (no 'MTrk'). :(\n");
       return;
   }
   // The length of the remaining data is a 4-byte, big-endian number
   unsigned int length = 0;
   for (int i = 0; i < 4; i++) {
      length = (length << 8) + data[18 + i];
   } 

   // now we can start reading MIDI info 

   for (int i = 22; i < length; i++) {
       // read time
       int bytes_read;
       unsigned int dt = read_variable_num(data + i, &bytes_read);
       i += bytes_read;
       // printf("delay: %d\n", dt);
       timer_delay_us(dt * tempo / 195);
       // read message type
       uint8_t msg_type = data[i];
       if ((msg_type >> 4) == 0x9 || 
           (msg_type >> 4) == 0x8 ||
           (msg_type >> 4) == 0xb ||
           (msg_type >> 4) == 0xe) {
           // two argument message
           midi_send(data + i, 3); 
           // midi_print(data + i, 3); 
           i += 2;
       } else if ((msg_type >> 4) == 0xc) {
           // one argument message
           midi_send(data + i, 2);
           // midi_print(data + i, 2);
           i++;
       } else if (msg_type == 0xff) {
           // meta message, which can be tempo 
           // we will ignore everything but the tempo
           uint8_t event = data[++i];
           i++;
           unsigned int length = read_variable_num(data + i, &bytes_read);
           i += bytes_read;
           if (event == 0x51) {
                // length number of bytes, in big-endian format, provide the tempo
                // in microseconds per MIDI quarter-note
                // (see here: http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html#BM3_)
                int new_tempo = 0;
                for (int j = 0; j < length; j++) {
                    new_tempo = (new_tempo << 8) + data[i + j];
                }
                // printf("new tempo: %d\n", new_tempo);
                tempo = new_tempo;
           }
           // skip over message
           i += length - 1;
       } else if (msg_type == 0xf0) {
           // "sysex" message, which we can ignore
           unsigned int length = read_variable_num(data + i, &bytes_read);
           i += bytes_read;
           // ignore
           i += length;
       } else {
           printf("Unknown message type! (0x%x)\n", msg_type);
       }
   } 
}

unsigned int read_variable_num(uint8_t *data, int *bytes_read) {
    unsigned int value;
    *bytes_read = 1; // we will read at least one byte
    value = data[0]; 
    // printf("v[0]: 0x%x, ", value); 
    if(value & 0x80) {
        value &= 0x7f;
        while (1) {
            uint8_t c = data[*bytes_read];
            (*bytes_read)++;
            // printf("0x%x, ", c);
            value = (value << 7) + (c & 0x7f);
            if (!(c & 0x80)) break;
        } 
    }
    // printf("\n");
    return(value);
}

