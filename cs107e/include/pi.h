#ifndef PI_H
#define PI_H

/*
 * Utility functions for Raspberry Pi.
 *
 * Authors: Pat Hanrahan <hanrahan@cs.stanford.edu>,
 *          Philip Levis <pal@cs.stanford.edu>,
 *          Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "gpio.h"

/*
 * Enumeration for valid LED identifiers.
 *
 * The two LEDs on the Raspberry Pi board are:
 *  PI_ACT_LED    green LED labeled `ACT`
 *  PI_PWR_LED    red LED labeled `PWR`
 */
enum {
    PI_ACT_LED = GPIO_PIN47,
    PI_PWR_LED = GPIO_PIN35
};

/*
 * `pi_reboot`
 *
 * Halts current program execution and reboots the Pi.
 */
void pi_reboot(void) __attribute__ ((noreturn));

/*
 * `pi_abort`
 *
 * Goes into an infininte loop that flashes the red power
 * LED on the Pi board. This function does not return.
 */
void pi_abort(void) __attribute__ ((noreturn));

/*
 * `pi_led_on`
 *
 * Turns on the specified LED.
 *
 * @param led      the led to turn on
 *
 * If `led` is not PI_ACT_LED or PI_PWR_LED, function does nothing.
 */
void pi_led_on(int led);

/*
 * `pi_led_off`
 *
 * Turns off the specified LED.
 *
 * @param led      the led to turn off
 *
 * If `led` is not PI_ACT_LED or PI_PWR_LED, function does nothing.
 */
void pi_led_off(int led);

/*
 * `pi_led_toggle`
 *
 * Toggles the specified LED. If on, turns it off. If off, turns on.
 *
 * @param led      the led to toggle
 *
 * If `led` is not PI_ACT_LED or PI_PWR_LED, function does nothing.
 */
void pi_led_toggle(int led);

#endif
