#include "midi.h"
#include "gpio.h"
#include "timer.h"

#define MIDI_PIN GPIO_PIN23
#define BAUD     35000
#define DELAY    (1000000 / BAUD)

static unsigned channel = 0;

void midi_init() {
  gpio_set_output(MIDI_PIN);
}

// Need inversion: a logical 1 is a high signal
inline void send_bit(uint8_t val) {
  gpio_pin_write(MIDI_PIN, val);
  timer_wait_for(DELAY);
}

inline void send_byte(uint8_t val) {
  unsigned i;
  send_bit(0); // start bit
  for (i = 0; i < 8; i++) { // data bits, LSB first
    send_bit((val >> i) & 1);
  }
  send_bit(1); // stop bit
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
