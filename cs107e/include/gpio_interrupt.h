#ifndef GPIO_INTERRUPT_H
#define GPIO_INTERRUPT_H

/*
 * Module to configure GPIO interrupt processing for Mango Pi.
 * Because all of the gpios within one group share the same interrupt
 * source, an additional level of indirection is needed to dispatch
 * those interrupts to individual handlers per specific gpio.
 * This module manages that additional layer of dispatch.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>
#include "gpio.h"
#include "interrupts.h"

/*
 * `gpio_interrupt_init`: Required initialization for module
 *
 * Initialize the GPIO interrupt module. The init function must be
 * called before any calls to other functions in this module.
 * The init function coordinates with the top-level interrupts module
 * to register a handler to receive events on all GPIO interrupt sources
 * (i.e. for all groups). The handler will re-dispatch each interrupt
 * to the per-gpio handler registered with this module.
 *
 * It is safe to call init more than once for this module. A re-init
 * retains all handlers that were previously registered.
 *
 * It is an error to attempt to initialize this module without first
 * initialized the top-level interrupts (i.e. required to call `interrupts_init`).
 */
void gpio_interrupt_init(void);

/*
 * `gpio_interrupt_register_handler`
 *
 * Register a handler function to gpio with id `pin`. Each GPIO
 * can have one handler.
 *
 * @param pin       the id of the GPIO pin
 * @param fn        handler function to call when interrupt generated on pin
 * @param aux_data  client's data pointer to be passed as second argument
 *                  when calling handler function
 *
 * Raises an assert if `pin` does not refer to valid gpio id. `aux_data`
 * can be NULL if handler function has no need for auxiliary data. If `fn`
 * is NULL, removes any handler previously registered for `pin`.
 *
 * It is an error to register handler without first
 * initializing the module (i.e. required to call `gpio_interrupt_init`).
 */
void gpio_interrupt_register_handler(gpio_id_t pin, handlerfn_t fn, void *aux_data);

/*
 * Enumeration for GPIO interrupt events
 *
 * The enumerated values below establish symbolic names for the different
 * GPIO events that can trigger an interrupt.
 */
typedef enum {
    GPIO_INTERRUPT_POSITIVE_EDGE = 0,
    GPIO_INTERRUPT_NEGATIVE_EDGE = 1,
    GPIO_INTERRUPT_HIGH_LEVEL = 2,
    GPIO_INTERRUPT_LOW_LEVEL = 3,
    GPIO_INTERRUPT_DOUBLE_EDGE = 4,
} gpio_event_t;

/*
 * `gpio_interrupt_config`
 *
 * Config interrupt to trigger on `event` for GPIO with id `pin`.
 * The boolean parameter `debounce` controls whether to apply debounce
 * circuit to filter/coalesce sequence of rapid-fire events. If true, debounce
 * filters to ~1 event/ms. If events expected to be more rapid (e.g. PS/2),
 * set debounce to false to not miss events.
 *
 * @param pin        the id of the GPIO pin
 * @param event      which event, see gpio_event_t enumeration
 * @param debounce   whether to apply debounce filter
 *
 * Raises an assert if `pin` or `event` is not valid.
 */
void gpio_interrupt_config(gpio_id_t pin, gpio_event_t event, bool debounce);

/*
 * `gpio_interrupt_enable`
 *
 * Enable interrupts for GPIO with id `pin`.
 *
 * @param pin      the id of the GPIO pin
 *
 * Raises an assert if `pin` is not valid.
 */
void gpio_interrupt_enable(gpio_id_t pin);

/*
 * `gpio_interrupt_disable`
 *
 * Disable interrupts for GPIO with id `pin`.
 *
 * @param pin      the id of the GPIO pin
 *
 * Raises an assert if `pin` is not valid.
 */
void gpio_interrupt_disable(gpio_id_t pin);

/*
 * `gpio_interrupt_clear`
 *
 * Clears any pending interrupt for GPIO with id `pin`.
 *
 * @param pin      the id of the GPIO pin
 *
 * Raises an assert if `pin` is not valid.
 */
void gpio_interrupt_clear(gpio_id_t pin);

#endif
