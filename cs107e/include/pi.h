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
 * The two LEDs on the Raspberry Pi board are:
 *  PI_ACT_LED    green LED labeled `ACT`
 *  PI_PWR_LED    red LED labeled `PWR`
 */
enum {
    PI_ACT_LED = GPIO_PIN47,
    PI_PWR_LED = GPIO_PIN35
};

/*
 * Halts current program execution and reboots the Pi.
 */
void pi_reboot(void);

/*
 * Goes into an infininte loop that flashes the red power
 * LED on the Pi board. This function does not return.
 */
void pi_abort(void);

/*
 * Turns on the specified LED.
 *
 * `led` must be either PI_ACT_LED or PI_PWR_LED. If not, this function does nothing.
 */
void pi_led_on(int led);

/*
 * Turns off the specified LED.
 *
 * `led` must be either PI_ACT_LED or PI_PWR_LED. If not, this function does nothing.
 */
void pi_led_off(int led);

/*
 * Toggles the specified LED. If on, turns it off. If off, turns on.
 *
 * `led` must be either PI_ACT_LED or PI_PWR_LED. If not, this function does nothing.
 */
void pi_led_toggle(int led);

#endif
