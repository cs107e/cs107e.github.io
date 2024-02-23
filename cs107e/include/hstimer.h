#ifndef HSTIMER_H
#define HSTIMER_H

/*
 * Functions for Mango Pi high-speed timer.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>

/*
 * `hstimer_init` : required module initialization
 *
 * Initialize the hstimer peripheral for use as a
 * "countdown" timer. The number of microsecs to count is specified
 * as the `us_interval` argument to `hstimer_init`.
 * The counter starts at that value and decrements to zero.
 * When the countdown hits zero, it will generate
 * an interrupt. Once the interrupt is cleared, the timer will
 * restart the cycle and count down again.
 *
 * @param us_interval       count of usecs per timer event
 */
void hstimer_init(int us_interval);

/*
 * `hstimer_enable`
 *
 * Enable hstimer. Count down will start/resume.
 */
void hstimer_enable(void);

/*
 * `hstimer_disable`
 *
 * Disable hstimer.  Suspends counting down.
 */
void hstimer_disable(void);

/*
 * `hstimer_interrupt_clear`
 *
 * Clear any pending hstimer interrupt.
 * Will restart count if hstimer is enabled.
 */
void hstimer_interrupt_clear(void);

#endif
