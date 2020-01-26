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
 * `armtimer_init`
 *
 * Initialize the armtimer peripheral which provides a
 * "countdown" timer. The number of ticks to count is specified
 * as the `tick_interval` argument to armtimer_init(). 
 * Once the initialized armtimer is enabled, it will start the counter 
 * at tick_internal and count down to zero. The function 
 * armtimer_get_count() can be used to read the current value of the
 * countdown timer. When the countdown hits zero, an overflow event
 * happens. If interrupts are enabled, it will also generate an 
 * interrupt on that event. At point of overflow/interrupt,
 * timer will restart counting down.
 */
void armtimer_init(unsigned int tick_interval);

/*
 * `armtimer_enable`
 *
 * Enable the armtimer countdown timer.  Starts counting down.
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
 * If interrupts are enabled, an interrupt is generated when the 
 * countdown timer reaches zero.
 */
void armtimer_enable_interrupts(void);

/*
 * `armtimer_disable_interrupts`
 *
 * If interrupts are disabled, no interrupt is generated when the 
 * countdown timer reaches zero.
 */
void armtimer_disable_interrupts(void);

/*
 * `armtimer_get_count`
 *
 * Returns the current value of the countdown timer. 
 */
unsigned int armtimer_get_count(void);

/*
 * `armtimer_check_overflow`
 *
 * Returns whether an overflow event has occurred (i.e. 
 * countdown has reached zero).
 * The return value is true if overflow event has occurred and 
 * the event has not been cleared, false otherwise.
 */
bool armtimer_check_overflow(void);

/*
 * `armtimer_clear_event`
 *
 * Clears event status. Any previous overflow event is cleared.
 */
void armtimer_clear_event(void);

/*
 * `armtimer_check_and_clear_overflow`
 *
 * Returns whether an overflow event occurred and clears the
 * event.
 */
bool armtimer_check_and_clear_overflow(void);

/*
 * `armtimer_check_interrupt`
 *
 * Returns whether an interrupt was generated (i.e. overflow 
 * event has occurred and interrupts are enabled).
 * The return value is true if interrupt has occurred and 
 * the event has not been cleared, false otherwise.
 */
bool armtimer_check_interrupt(void);

/*
 * `armtimer_check_and_clear_interrupt`
 *
 * Returns whether an interrupt was generated and clears the
 * event.
 */
bool armtimer_check_and_clear_interrupt(void);

#endif
