#ifndef TIMER_H
#define TIMER_H

/*
 * Hardware abstraction for a timer.
 *
 * Students implement this module in assignment 2.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#define TICKS_PER_USEC 24 // 24 ticks are equal to one microsecond

/*
 * `timer_init`
 *
 * Initialize the timer. For assignment 2, this does nothing.
 * However, all peripheral modules require an init, so it is
 * included for consistency's sake.
 */
void timer_init(void);

/*
 * `timer_get_ticks`
 *
 * Returns the current system tick count. The tick count is set to
 * zero at boot and increments at constant rate 24Mhz until the
 * processor resets.
 *
 * @return  system tick count
 */
unsigned long timer_get_ticks(void);

/*
 * `timer_delay_us`
 *
 * Delay the program for `usec` microseconds in a busy loop.
 *
 * @param usec  the number of microseconds to delay
 */
void timer_delay_us(int usec);

/*
 * `timer_delay_ms`
 *
 * Delay for `msec` milliseconds.
 *
 * @param msec  the number of milliseconds to delay
 */
void timer_delay_ms(int msec);

/*
 * `timer_delay`
 *
 * Delay for `sec` seconds.
 *
 * @param sec   the number of seconds to delay
 */
void timer_delay(int sec);

#endif
