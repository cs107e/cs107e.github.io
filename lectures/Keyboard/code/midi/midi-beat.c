#include "timer.h"
#include "gpio.h"
#include "midi.h"
#include "pi.h"

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

struct beat_timing {
  unsigned int delay;
  uint8_t beat;
};

struct beat_timing run[] = {
	{1000, BEATS_KICK},
        {1000, BEATS_SNARE},
        {1000, BEATS_LOTOM},
        {1000, BEATS_HITOM},
        {1000, BEATS_CLHAT},
        {1000, BEATS_OPHAT},
        {1000, BEATS_CLAP}
};

struct beat_timing beats[] = {
        {500, BEATS_KICK},
	{500, BEATS_SNARE},
	{150, BEATS_KICK},
	{350, BEATS_KICK},
	{150, BEATS_SNARE},
	{350, BEATS_OPHAT},
};

void main(void) {
  timer_init();
  gpio_init();
  midi_init();
  midi_set_channel(CHANNEL_BEATS);
  
  while (1) {
    for (unsigned int i = 0; i < sizeof(beats) / sizeof(struct beat_timing); i++) {
      midi_note_on(beats[i].beat, 64);
      timer_delay_ms(100);
      midi_note_off(beats[i].beat, 64);
      timer_delay_ms(beats[i].delay - 100);
      pi_led_toggle(PI_ACT_LED);
    }
  }
  
 /* while (1) {
    for (unsigned int i = 20; i < 80; i += 5) {
      midi_note_on(i, 64);
      timer_delay_ms(250);
      midi_note_off(i, 64);
      pi_led_toggle(PI_ACT_LED);
    }
  }*/
}
