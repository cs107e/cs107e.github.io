#ifndef ARMTIMER_REF
#define ARMTIMER_REF
#include "armtimer.h"
void ref_armtimer_init(unsigned int tick_interval);
void ref_armtimer_enable(void);
void ref_armtimer_disable(void);
void ref_armtimer_enable_interrupts(void);
void ref_armtimer_disable_interrupts(void);
unsigned int ref_armtimer_get_count(void);
bool ref_armtimer_check_overflow(void);
void ref_armtimer_clear_event(void);
bool ref_armtimer_check_and_clear_overflow(void);
bool ref_armtimer_check_interrupt(void);
bool ref_armtimer_check_and_clear_interrupt(void);
#endif
