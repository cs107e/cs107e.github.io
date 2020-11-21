#ifndef GPIO_INTERRUPTS_H
#define GPIO_INTERRUPTS_H

#include <stdbool.h>
#include "interrupts.h"

/*
 * Module to configure GPIO interrupts for Raspberry Pi.
 * Because all of the GPIO pins share a small set of GPIO
 * interrupts, you need a level of indirectiom to be able
 * to handle interrupts for particular pins. This module
 * allows a client to register one handler for each
 * GPIO pin.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 *
 * Last update:   May 2020
 */

/*
 * `gpio_interrupts_init`
 *
 * Initialize the GPIO interrupt modules. The init function must be
 * called before any calls to other functions in this module.
 * The init function configures gpio interrupts to a clean state.
 * This function registers a handler for GPIO interrupts, which it
 * uses to call handlers on a per-pin basis.
 *
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
 * `interrupts_register_handler`
 *
 * Register a handler function to a given GPIO pin. Each pin
 * source can have one handler: further dispatch should be invoked by
 * the handler itself. Whether or not a particular pin will
 * generate interrupts is specified by the events system,
 * defined in `gpioextra.h`.
 *
 * Asserts if failed to install handler (e.g., the pin is invalid).
 * Pins are defined in `gpio.h`.
 */
handler_fn_t gpio_interrupts_register_handler(unsigned int pin, handler_fn_t fn);

/*
 * `gpio_interrupts_default_handler`
 *
 * The default handler for GPIO events. Does nothing. Provided
 * as reference point for what the prior handler returned from
 * registering a handler, plus as a convenience for uninstalling
 * a handler (replace it with this).
 */
bool gpio_default_handler(unsigned int pc);


#endif
