#ifndef HSTIMER_H
#define HSTIMER_H

/*
 * Functions for Mango Pi high-speed timer.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>

/*
 * Type: `hstimer_id_t`
 *
 * The hstimer peripheral has two available timers: HSTIMER0 and HSTIMER1.
 */
typedef enum  { HSTIMER0 = 0, HSTIMER1 = 1 } hstimer_id_t;

/*
 * `hstimer_init` : required module initialization
 *
 * Initialize the hstimer peripheral `timer` for use as a "countdown" timer.
 * The number of microsecs to count is specified  as the `us_interval`
 * argument. A request to init (or re-init) hstimer will configure
 * the countdown interval and place the timer into a disabled state.
 * Use `hstimer_enable` to begin counting down when ready.
 *
 * When an hstimer is enabled, its counter is decremented once per microsec.
 * When counter reaches zero, it will generate an interrupt. Clearing the
 * interrupt will restart the timer, causing it to countdown again from the
 * initial interval value. The cycle repeats until timer is disabled.
 *
 * The `timer` argument specifies which hstimer id to operate on. A hstimer id
 * that is not valid will be ignored.
 *
 * @param timer         which hstimer to config (either HSTIMER0 or HSTIMER1)
 * @param us_interval   count of usecs per timer interval
 */
void hstimer_init(hstimer_id_t timer, long us_interval);

/*
 * `hstimer_enable`
 *
 * Enable `timer`. Countdown will start/resume.
 *
 * @param timer         which hstimer to enable (either HSTIMER0 or HSTIMER1)
 */
void hstimer_enable(hstimer_id_t timer);

/*
 * `hstimer_disable`
 *
 * Disable `timer`. Suspends counting down.
 *
 * @param timer         which hstimer to disable (either HSTIMER0 or HSTIMER1)
 */
void hstimer_disable(hstimer_id_t timer);

/*
 * `hstimer_interrupt_clear`
 *
 * Clear any pending hstimer interrupt for `timer`. Countdown will restart
 * from the initial interval.
 *
 * @param timer         which hstimer to enable (either HSTIMER0 or HSTIMER1)
 */
void hstimer_interrupt_clear(hstimer_id_t timer);

#endif
