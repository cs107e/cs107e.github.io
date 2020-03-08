#include "timer.h"
#include "gpio.h"
#include "gpioextra.h"
#include "midi.h"

#define MIDI_PIN GPIO_PIN25
#define BAUD     31250
#define DELAY    (1000000 / BAUD)

static unsigned int channel = 0;

void midi_init(void) {
  gpio_set_output(MIDI_PIN);
  gpio_write(MIDI_PIN, 1);
  //gpio_set_pulldown(MIDI_PIN);
}

inline void send_bit(uint8_t val) {
  gpio_write(MIDI_PIN, val);
  timer_delay_us(DELAY);
}

inline void send_byte(uint8_t val) {
  send_bit(0); // start bit
  for (unsigned int i = 0; i < 8; i++) { // data bits, LSB first
    send_bit((val >> i) & 1);
  }
  send_bit(1); // stop bit
}

void midi_send(uint8_t* cmd, unsigned int len) {
  for (unsigned int i = 0; i < len; i++) {
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

void midi_test(void) {
  unsigned command = 0xaaaa5555;
  midi_send((uint8_t*)&command, 4);
}

void midi_set_channel(unsigned int ch) {
  channel = ch;
}

unsigned int midi_channel(void) {
  return channel;
}
