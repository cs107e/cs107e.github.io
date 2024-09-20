#ifndef GPIO_EXTRA_H
#define GPIO_EXTRA_H

/*
 * Functions for controlling extra GPIO features (pull state and name to id conversion)
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

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
