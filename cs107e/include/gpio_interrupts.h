#ifndef GPIO_INTERRUPTS_H
#define GPIO_INTERRUPTS_H

#include "interrupts.h"

/*
 * Module to configure GPIO interrupts for Raspberry Pi.
 * Because all of the GPIO pins share a small set of GPIO
 * interrupts, you need a level of indirection to be able
 * to handle interrupts for particular pins. This module
 * allows a client to register one handler for each
 * GPIO pin.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 *
 * Last update:   May 2020
 */

/*
 * `gpio_interrupts_init`: Required initialization for module
 *
 * Initialize the GPIO interrupts module. The init function must be
 * called before any calls to other functions in this module.
 * The init function coordinates with the top-level interrupts module.
 * It registers a dispatch handler with the top-level interrupts module.
 * This dispatch handler receives all GPIO interrupts at the top-level
 * and in turn dispatches to the handler registered with this module.
 * The handlers registered with this module are associated with a
 * specific GPIO pin.
 *
 * When this function returns, GPIO interrupts are disabled and
 * must be enabled with `gpio_interrupts_enable`.
 *
 * It is safe to call init more than once for this module. A re-init
 * retains all handlers that were previously registered.
 *
 * Note: the top-level interrupts module must be initialized before
 * attempting to initialize this module.
 */
void gpio_interrupts_init(void);

/*
 * `gpio_interrupts_enable`
 *
 * Global enable for GPIO interrupts.
 */
void gpio_interrupts_enable(void);

/*
 * `gpio_interrupts_disable`
 *
 * Global disable for GPIO interrupts.
 */
void gpio_interrupts_disable(void);

/*
 * `gpio_interrupts_register_handler`
 *
 * Register a handler function to a given GPIO pin. Each GPIO
 * pin can have one handler: further dispatch should be invoked by
 * the handler itself. Use the events system defined in `gpio_extra.h`
 * to configure which GPIO events are detected by the pin.
 *
 * @param pin       GPIO pin to register handler
 * @param fn        handler function to call when interrupt generated on pin
 * @param aux_data  client's data pointer to be passed as second argument
 *                  when calling handler function
 *
 * An assert is raised if `pin` is invalid. `aux_data` can be NULL if
 * handler function has no need for auxiliary data. If `fn` is NULL, removes
 * any handler previously registered for `pin`.
 *
 * This function asserts on an attempt to register handler without first
 * initializing the module (i.e. required to call `gpio_interrupts_init`).
 */
void gpio_interrupts_register_handler(unsigned int pin, handler_fn_t fn, void *aux_data);


#endif
