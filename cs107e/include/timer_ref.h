#ifndef TIMER_REF
#define TIMER_REF
#include "timer.h"
void ref_timer_init(void);
unsigned int ref_timer_get_ticks(void);
void ref_timer_delay_us(unsigned int usec);
void ref_timer_delay_ms(unsigned int msec);
void ref_timer_delay(unsigned int sec);
#endif
