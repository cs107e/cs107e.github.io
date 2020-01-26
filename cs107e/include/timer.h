#ifndef TIMER_H
#define TIMER_H

/*
 * Hardware abstractions for a Raspberry Pi timer.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 *         Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Date: Jan 24, 2016
 * Edited by Mark Miller, Anna Zeng, Jan 21, 2018
 */

/*
 * Initialize the timer. For assignment 2, this does nothing.
 * However, all libpi peripheral modules require an init, so it is
 * included for consistency's sake.
 */
void timer_init(void);

/*
 * Returns the current system tick count. The tick count is set to
 * zero at boot and increments once per microsecond until the
 * processor resets.  One tick is equal to one microsecond.
 *
 * @return  system tick count
 */
unsigned int timer_get_ticks(void);

/*
 * A simple busy loop that delays the program for `usec` microseconds.
 *
 * @param usec  the number of microseconds to busy loop for
 */
void timer_delay_us(unsigned int usec);

/*
 * Delay for `msec` milliseconds.
 *
 * @param msec  the number of milliseconds to busy loop for
 */
void timer_delay_ms(unsigned int msec);

/*
 * Delay for `sec` seconds.
 *
 * @param sec the number of seconds to busy loop for
 */
void timer_delay(unsigned int sec);

#endif
