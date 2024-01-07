#ifndef MANGO_H
#define MANGO_H

/*
 * Utility functions for Mango Pi.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */
#include "gpio.h"

/*
 * The blue act led on-board the Mango Pi is controlled via GPIO PD18.
 */
 #define MANGO_ACTLED GPIO_PD18

/*
 * Enumeration for LED state.
 */
enum led_state_t {
    LED_OFF = 0,
    LED_ON,
    LED_TOGGLE
};

/*
 * `mango_actled`
 *
 * Change state of the act led.
 *
 * @param state      state change for led
 *
 * If state is not valid, function does nothing.
 */
void mango_actled(enum led_state_t state);

/*
 * `mango_reboot`
 *
 * Halts current program execution and reboots the Pi.
 */
void mango_reboot(void) __attribute__ ((noreturn));

/*
 * `mango_abort`
 *
 * Goes into an infininte loop that flashes an SOS
 * pattern on the act led. This function does not return.
 */
void mango_abort(void) __attribute__ ((noreturn));

#endif
