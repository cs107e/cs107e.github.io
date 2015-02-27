#include "gpio.h"
#include "timer.h"
#include "midi.h"


#define CHANNEL_KEY    0
#define CHANNEL_BEATS  9

typedef enum {
  BEATS_KICK = 36,
  BEATS_SNARE =  38,
  BEATS_LOTOM = 43,
  BEATS_HITOM = 50,
  BEATS_CLHAT = 42,
  BEATS_OPHAT = 46,
  BEATS_CLAP  = 39
} beats_t;

uint8_t beats[] = {
  BEATS_KICK,
  BEATS_SNARE,
  BEATS_LOTOM,
  BEATS_HITOM,
  BEATS_CLHAT,
  BEATS_OPHAT,
  BEATS_CLAP
};

void notmain() {
  gpio_init();
  timer_init();
  midi_init();
  midi_set_channel(CHANNEL_BEATS);

  while (1) {
    unsigned i;
    for (i = 0; i < sizeof(beats); i++) {
      midi_note_on(beats[i], 64);
      timer_wait_for(250000);
      midi_note_off(beats[i], 64);
      led_toggle();
    }
  }
  
  while (1) {
    unsigned i;
    for (i = 20; i < 80; i += 5) {
      midi_note_on(i, 64);
      timer_wait_for(250000);
      midi_note_off(i, 64);
      led_toggle();
    }
  }
}
