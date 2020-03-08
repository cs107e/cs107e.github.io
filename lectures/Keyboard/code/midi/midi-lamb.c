#include "timer.h"
#include "gpio.h"
#include "midi.h"
#include "pi.h"

#define CHANNEL_BEATS  9

typedef enum {
  ZERO = 0,
  G3 = 55,
  A3 = 57,
  B3 = 59,
  C4 = 60,
  D4 = 62,
  E4 = 64,
  F4 = 65
} notes_t;

typedef struct {
  notes_t note;
  unsigned time;
} press_t;

#define HLF_NOTE 1000000
#define QTR_NOTE 500000
#define PAUSE    200000

press_t notes[] = {
  {B3, QTR_NOTE},
  {A3, QTR_NOTE},
  {G3, QTR_NOTE},
  {A3, QTR_NOTE},
  {B3, QTR_NOTE},
  {B3, QTR_NOTE},
  {B3, HLF_NOTE},
  {A3, QTR_NOTE},
  {A3, QTR_NOTE},
  {A3, HLF_NOTE},
  {B3, QTR_NOTE},
  {D4, QTR_NOTE},
  {D4, HLF_NOTE},
  {B3, QTR_NOTE},
  {A3, QTR_NOTE},
  {G3, QTR_NOTE},
  {A3, QTR_NOTE},
  {B3, QTR_NOTE},
  {B3, QTR_NOTE},
  {B3, QTR_NOTE},
  {B3, QTR_NOTE},
  {A3, QTR_NOTE},
  {A3, QTR_NOTE},
  {B3, QTR_NOTE},
  {A3, QTR_NOTE},
  {G3, HLF_NOTE},
};
  

void main(void) {
  timer_init();
  gpio_init();
  midi_init();
  
  while (1) {
    unsigned int len = sizeof(notes) / sizeof(press_t);
    for (unsigned int i = 0; i < len; i ++) {
      press_t* press = &notes[i];
      midi_note_on(press->note, 64);
      timer_delay_us(press->time);
      midi_note_off(press->note, 64);
      pi_led_toggle(PI_ACT_LED);
    }
    timer_delay_ms(4000);
  }
}
