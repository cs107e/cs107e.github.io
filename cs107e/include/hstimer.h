#ifndef HSTIMER_H
#define HSTIMER_H

/*
 * Functions for Mango Pi high-speed timer.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>
#include "interrupts.h"

/*
 * Type: `hstimer_id_t`
 *
 * The hstimer peripheral has two available timers: HSTIMER0 and HSTIMER1.
 */
typedef enum  { HSTIMER0 = 0, HSTIMER1 = 1 } hstimer_id_t;

/*
 * Type: `hstimer_mode_t`
 *
 * The hstimer can operate in one of two modes: periodic or one shot.
 * If periodic, the countdown is restarted after clearing each event.
 * If oneshot, it counts down once.
 */
typedef enum  { HSTIMER_PERIODIC = 0, HSTIMER_ONESHOT = 1 } hstimer_mode_t;


/*
 * `hstimer_init` : required module initialization
 *
 * Initialize the hstimer peripheral `timer` for use as a "countdown" timer.
 * The number of microsecs to count is specified  as the `us_interval`
 * argument. A request to init (or re-init) hstimer will configure
 * the countdown interval and place the timer into a disabled state.
 * Use `hstimer_enable` to begin counting down when ready.
 *
 * Once the hstimer is enabled, its counter is decremented once per microsec.
 * When counter reaches zero, it will generate an interrupt. If in periodic
 * mode, clearing the event resets countdown to the initial interval value
 * and countdown begins anew. The periodic cycle repeats until hstimer is disabled.
 * If mode is one shot, it counts down once and then disables hstimer.
 * A call to `hstimer_enable` will manually restart a one-shot timer.
 *
 * @param timer         which hstimer to config (either HSTIMER0 or HSTIMER1)
 * @param us_interval   count of usecs per timer interval
 * @param mode          whether timer operates in PERIODIC or ONE_SHOT mode
 */
void hstimer_init(hstimer_id_t timer, long us_interval, hstimer_mode_t mode);

/*
 * `hstimer_enable`
 *
 * Enable `timer`. Countdown will start/resume. An invalid hstimer id is ignored.
 *
 * @param timer         which hstimer to enable (either HSTIMER0 or HSTIMER1)
 */
void hstimer_enable(hstimer_id_t timer);

/*
 * `hstimer_disable`
 *
 * Disable `timer`. Suspends counting down. An invalid hstimer id is ignored.
 *
 * @param timer         which hstimer to disable (either HSTIMER0 or HSTIMER1)
 */
void hstimer_disable(hstimer_id_t timer);

/*
 * `hstimer_interrupt_clear`
 *
 * Clear any pending hstimer interrupt for `timer`. If the timer is operating in
 * periodic mode, the countdown will restart from the initial interval.
 *
 * @param timer         which hstimer to enable (either HSTIMER0 or HSTIMER1)
 */
void hstimer_interrupt_clear(hstimer_id_t timer);

/*
 * `hstimer_set_handler`
 *
 * Set the handler function for on `timer` and enables interrupt processing
 * of timer events.
 *
 * @param timer         which hstimer (either HSTIMER0 or HSTIMER1)
 * @param fn            handler function to call for event on hstimer
 * @param aux_data      client's data pointer to be passed as second argument
 *                      when calling handler function
 *
 * An invalid hstimer id is ignored. `aux_data` can be NULL if
 * the handler function has no need for auxiliary data.
 *
 * Pass NULL for `fn` to remove handler that was previously set.
 * No interrupts are generated when the hstimer has no enabled handler.
 */
void hstimer_set_handler(hstimer_id_t index, handlerfn_t fn, void *client_data);

#endif
