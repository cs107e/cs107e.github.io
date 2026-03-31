#ifndef GPIO_INTERRUPT_H
#define GPIO_INTERRUPT_H

/*
 * Module to configure GPIO interrupt processing for Mango Pi.
 * Because all of the gpios within one group share the same interrupt
 * source, an additional level of indirection is needed to dispatch
 * interrupts to individual handlers per specific gpio.
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
 * to set a handler to receive events on all GPIO interrupt sources
 * (i.e. for all groups). The handler will re-dispatch the interrupt
 * to the per-gpio handler managed by this module.
 *
 * It is safe to call init more than once for this module. A re-init
 * retains all handlers that were previously set.
 *
 * It is an error to attempt to initialize this module without first
 * initialized the top-level interrupts (i.e. required to call `interrupts_init`).
 */
void gpio_interrupt_init(void);

/*
 * Enumeration for GPIO interrupt events
 *
 * The enumerated values below establish symbolic names for the different
 * GPIO events that can trigger an interrupt. "Positive" edge is what we call
 " rising edge, "negative" is falling. "Double edge" triggers for either
 * rising or falling.
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
 * Config settings for gpio interrupt to trigger on `event` for GPIO id `pin`.
 * Set the gpio function of `pin` to `GPIO_FN_INTERRUPT` and select
 * which type of event will trigger an interrupt.
 * The boolean parameter `debounce` controls whether to apply debounce
 * circuit to filter/coalesce sequence of rapid-fire events. Debounce true
 * will filter repeated events to about 1 event/ms. If events are expected
 * to arrive more rapidly (e.g. PS/2 10Khz = 10 events/ms), set debounce
 * to false so all events will be passed through as-is, no filtering.
 *
 * @param pin        the id of the GPIO pin
 * @param event      which event, see gpio_event_t enumeration
 * @param debounce   whether to apply debounce filter
 *
 * Raises an assert if `pin` or `event` is not valid.
 */
void gpio_interrupt_config(gpio_id_t pin, gpio_event_t event, bool debounce);

/*
 * `gpio_interrupt_set_handler`
 *
 * Set the handler function for events on GPIO with id `pin` and enables
 * interrupt processing of events as configured by earlier call to
 * `gpio_interrupt_config`. When a future event is generated
 * for `pin`, the handler is called to process it.
 *
 * @param pin       the id of the GPIO pin
 * @param fn        handler function to call when interrupt generated on pin
 * @param aux_data  client's data pointer to be passed as second argument
 *                  when calling handler function
 *
 * Raises an assert if `pin` does not refer to valid gpio id. `aux_data`
 * can be NULL if handler function has no need for auxiliary data.
 *
 * Pass NULL for `fn` to disable events for pin and remove handler that was previously set.
 * No interrupts are generated when pin has no enabled handler.
 */
void gpio_interrupt_set_handler(gpio_id_t pin, handlerfn_t fn, void *aux_data);

/*
 * `gpio_interrupt_clear`
 *
 * Clears any pending interrupt for GPIO with id `pin`. Call this function
 * to reset interrupt state for this pin, such as after the event has been
 * handled or when resetting the pin's interrupt configuration.
 *
 * @param pin      the id of the GPIO pin
 *
 * Raises an assert if `pin` is not valid.
 */
void gpio_interrupt_clear(gpio_id_t pin);

#endif
