#ifndef GPIO_EXTRA_H
#define GPIO_EXTRA_H

/*
 * Functions for controlling extra GPIO features (pull state and interrupts)
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>
#include "gpio.h"

/*
 * The functions below control the pull state of a GPIO pin via its
 * internal resistor. Setting the pull state is useful for when you
 * have an input pin that would otherwise be floating. A pin configured
 * to pull-up defaults to 1. If configured to pull-down defaults to 0.
 */

/*
 * `gpio_set_pullup`
 *
 * Set pull state of GPIO with id `pin` to pull-up.
 *
 * @param pin   the id of the GPIO pin
 *
 * If `pin` is not valid, does nothing.
 */
void gpio_set_pullup(gpio_id_t pin);

/*
 *
 * `gpio_set_pulldown`
 *
 * Set pull state of GPIO with id `pin` to pull-down.
 *
 * @param pin   the id of the GPIO pin
 *
 * If `pin` is not valid, does nothing.
 */
void gpio_set_pulldown(gpio_id_t pin);

/*
 * `gpio_set_pullnone`
 *
 * Disables any pull state of GPIO with id `pin` (value will float).
 *
 * @param pin   the id of the GPIO pin
 *
 * If `pin` is not valid, does nothing.
 */
void gpio_set_pullnone(gpio_id_t pin);

/*
 * Enumeration for GPIO interrupt events
 *
 * The enumerated values below establish symbolic names for the different
 * GPIO events that can trigger an interrupt.
 */
enum gpio_interrupt_event_t {
    GPIO_INTERRUPT_POSITIVE_EDGE = 0,
    GPIO_INTERRUPT_NEGATIVE_EDGE = 1,
    GPIO_INTERRUPT_HIGH_LEVEL = 2,
    GPIO_INTERRUPT_LOW_LEVEL = 3,
    GPIO_INTERRUPT_DOUBLE_EDGE = 4,
};

/*
 * `gpio_interrupt_set_trigger`
 *
 * Set interrupt to trigger on `event` for GPIO with id `pin`.
 *
 * @param pin      the id of the GPIO pin
 * @param event    which event, see gpio_interrupt_event enumeration
 *
 * If `pin` or `event` is not valid, does nothing.
 */
void gpio_interrupt_set_trigger(gpio_id_t pin, unsigned int event);

/*
 * `gpio_interrupt_enable`
 *
 * Enable interrupts for GPIO with id `pin`.
 *
 * @param pin      the id of the GPIO pin
 *
 * If `pin`  is not valid, does nothing.
 */
void gpio_interrupt_enable(gpio_id_t pin);

/*
 * `gpio_interrupt_disable`
 *
 * Disable interrupts for GPIO with id `pin`.
 *
 * @param pin      the id of the GPIO pin
 *
 * If `pin` is not valid, does nothing.
 */
void gpio_interrupt_disable(gpio_id_t pin);

/*
 * `gpio_interrupt_is_pending`
 *
 * Query whether there is pending interrupt for GPIO with id `pin`.
 * Returns true if an interrupt is pending for `pin`
 * and has not been cleared, false otherwise.
 *
 * @param pin   the id of the GPIO pin
 * @return      true if interrupt pending, false otherwise
 *
 * If `pin` is not valid, returns false.
 */
bool gpio_interrupt_is_pending(gpio_id_t pin);

/*
 * `gpio_interrupt_clear`
 *
 * Clears any pending interrupt for GPIO with id `pin`.
 *
 * @param pin      the id of the GPIO pin
 *
 * If `pin` is not valid, does nothing.
 */
bool gpio_interrupt_clear(gpio_id_t pin);

/*
 * `gpio_interrupt_source`
 *
 * Returns the interrupt source for GPIO with id `pin`,
 * suitable for being used to register a handler with the
 * interrupt system.
 *
 * @param pin      the id of the GPIO pin
 *
 * If `pin` is not valid, returns GPIO_INVALID_REQUEST.
 */
int gpio_interrupt_source(gpio_id_t pin);

/*
 * `gpio_get_name_for_id`
 *
 * Translate a pin id into name string suitable for display:
 * e.g. GPIO_PB4 -> "PB4"
 *
 * @param pin      the id of the GPIO pin
 * @return         name string
 *
 * If `pin` is not valid, returns GPIO_INVALID_REQUEST.
 */
const char *gpio_get_name_for_id(gpio_id_t pin);

/*
 * `gpio_get_id_for_name`
 *
 * Translate a string name into pin id:
 * e.g. "PB4" -> GPIO_PB4
 *
 * @param name  name string
 * @return      the id of the GPIO pin
 *
 * If `name` is not valid, returns GPIO_INVALID_REQUEST.
 */
gpio_id_t gpio_get_id_for_name(const char *name);

#endif
