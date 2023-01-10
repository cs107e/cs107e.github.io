#ifndef ARMTIMER_H
#define ARMTIMER_H

#include <stdbool.h>

/*
 * Functions for Raspberry Pi ARM Timer.
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 *         Dawson Engler <engler@cs.stanford.edu>
 *         Julie Zelenski <zelenski@cs.stanford.edu>
 *
 * Last updated: Feb 2018
 */

/*
 * `armtimer_init` : required module initialization
 *
 * Initialize the armtimer peripheral for use as a
 * "countdown" timer. The number of ticks to count is specified
 * as the `tick_interval` argument to `armtimer_init`.
 * Use `armtimer_enable` to start the timer. The count
 * begins as `tick_internal` and decrements to zero. Call
 * `armtimer_get_count` to read the current value of the
 * countdown timer. When the countdown hits zero, an overflow event
 * happens. If armtimer interrupts are enabled, it will also generate
 * an interrupt for the event. At point of overflow/interrupt,
 * timer will restart the cycle and count down again.
 *
 * @param tick_interval       count of ticks per timer event
 */
void armtimer_init(unsigned int tick_interval);

/*
 * `armtimer_enable`
 *
 * Enable the armtimer countdown timer.  Starts counting down.
 * When count reaches zero, an overflow event occurs.
 */
void armtimer_enable(void);

/*
 * `armtimer_disable`
 *
 * Disable the armtimer countdown timer.  Suspends counting down.
 */
void armtimer_disable(void);

/*
 * `armtimer_enable_interrupts`
 *
 * Enable generation of armtimer interrupts. When an overflow event
 * occurs (count reaches zero), an interrupt is generated.
 */
void armtimer_enable_interrupts(void);

/*
 * `armtimer_disable_interrupts`
 *
 * Disable generation of armtimer interrupts.
 */
void armtimer_disable_interrupts(void);

/*
 * `armtimer_get_count`
 *
 * Retrieve current value of count.
 *
 * @return      current value of countdown timer
 */
unsigned int armtimer_get_count(void);

/*
 * `armtimer_check_overflow`
 *
 * Check whether an overflow event has occurred (i.e.
 * countdown has reached zero) and not been cleared.
 *
 * @return      true if overflow event has occurred and not cleared
 *              false otherwise
 */
bool armtimer_check_overflow(void);

/*
 * `armtimer_clear_event`
 *
 * Clear event status. Any previous overflow event is cleared.
 */
void armtimer_clear_event(void);

/*
 * `armtimer_check_and_clear_overflow`
 *
 * Check whether an overflow event occurred and if so, clear event.
 *
 * @return      true if overflow event occurred and was cleared
 *              false otherwise
 */
bool armtimer_check_and_clear_overflow(void);

/*
 * `armtimer_check_interrupt`
 *
 * Check whether armtimer interrupt was generated and
 * interrupt has not yet been cleared.
 *
 * @return      true if interrupt generated and not yet cleared
 *              false otherwise
 */
bool armtimer_check_interrupt(void);

/*
 * `armtimer_check_and_clear_interrupt`
 *
 * Check whether armtimer interrupt generated and if so,
 * clear interrupt.
 *
 * @return      true if interrupt generated and was cleared
 *              false otherwise
 */
bool armtimer_check_and_clear_interrupt(void);

#endif
