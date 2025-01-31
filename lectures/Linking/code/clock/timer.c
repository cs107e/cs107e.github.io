#include "timer.h"

unsigned long timer_get_ticks(void) {
    return 0;
}

void timer_init(void) {
    // no initialization required for this peripheral
}

void timer_delay(int secs) {
    timer_delay_us(secs*1000000);
}

void timer_delay_ms(int ms) {
    timer_delay_us(ms*1000);
}

void timer_delay_us(int us) {
    unsigned long busy_wait_until = timer_get_ticks() + us * TICKS_PER_USEC;
    while (timer_get_ticks() < busy_wait_until) {} // spin
}