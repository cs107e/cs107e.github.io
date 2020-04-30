#ifndef GPIO_H
#define GPIO_H

/*
 * Functions for controlling Raspberry Pi GPIO.
 * You will implement this interface in assignment 2.
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 *         Philip Levis <pal@cs.stanford.edu>
 *
 * Date: Jan 24, 2016
 * Edited by Mark Miller, Anna Zeng, Jan 21, 2018
 */


/*
 * These enumerated values are used to represent the different GPIO pins.
 * What makes an enumeration better than the number it represents? Why do we 
 * do this?
 *
 * Note also the values GPIO_PIN_FIRST and GPIO_PIN_LAST that bound
 * the range of valid GPIO pin numbers.
 */
enum {
    GPIO_PIN_FIRST = 0,
    GPIO_PIN0 = 0,
    GPIO_PIN1 = 1,
    GPIO_PIN2 = 2,
    GPIO_PIN3 = 3,
    GPIO_PIN4 = 4,
    GPIO_PIN5 = 5,
    GPIO_PIN6 = 6,
    GPIO_PIN7 = 7,
    GPIO_PIN8 = 8,
    GPIO_PIN9 = 9,
    GPIO_PIN10 = 10,
    GPIO_PIN11 = 11,
    GPIO_PIN12 = 12,
    GPIO_PIN13 = 13,
    GPIO_PIN14 = 14,
    GPIO_PIN15 = 15,
    GPIO_PIN16 = 16,
    GPIO_PIN17 = 17,
    GPIO_PIN18 = 18,
    GPIO_PIN19 = 19,
    GPIO_PIN20 = 20,
    GPIO_PIN21 = 21,
    GPIO_PIN22 = 22,
    GPIO_PIN23 = 23,
    GPIO_PIN24 = 24,
    GPIO_PIN25 = 25,
    GPIO_PIN26 = 26,
    GPIO_PIN27 = 27,
    GPIO_PIN28 = 28,
    GPIO_PIN29 = 29,
    GPIO_PIN30 = 30,
    GPIO_PIN31 = 31,
    GPIO_PIN32 = 32,
    GPIO_PIN33 = 33,
    GPIO_PIN34 = 34,
    GPIO_PIN35 = 35,
    GPIO_PIN36 = 36,
    GPIO_PIN37 = 37,
    GPIO_PIN38 = 38,
    GPIO_PIN39 = 39,
    GPIO_PIN40 = 40,
    GPIO_PIN41 = 41,
    GPIO_PIN42 = 42,
    GPIO_PIN43 = 43,
    GPIO_PIN44 = 44,
    GPIO_PIN45 = 45,
    GPIO_PIN46 = 46,
    GPIO_PIN47 = 47,
    GPIO_PIN48 = 48,
    GPIO_PIN49 = 49,
    GPIO_PIN50 = 50,
    GPIO_PIN51 = 51,
    GPIO_PIN52 = 52,
    GPIO_PIN53 = 53,
    GPIO_PIN_LAST =  53
};

// GPIO pin mappings for UART
#define GPIO_TX GPIO_PIN14
#define GPIO_RX GPIO_PIN15

/*
 * These enumerations give names for the functions of each GPIO pin. See the
 * Broadcom chip manual for more details, in particular Chapter 6: GPIO.
 */
enum {
    GPIO_FUNC_INPUT   = 0,
    GPIO_FUNC_OUTPUT  = 1,
    GPIO_FUNC_ALT0    = 4,
    GPIO_FUNC_ALT1    = 5,
    GPIO_FUNC_ALT2    = 6,
    GPIO_FUNC_ALT3    = 7,
    GPIO_FUNC_ALT4    = 3,
    GPIO_FUNC_ALT5    = 2,
};

#define GPIO_INVALID_REQUEST -1   // return value for invalid request


/*
 * Initialize the GPIO code module. For assignment 2, this does nothing.
 * However, all libpi peripheral modules require an init, so it is included
 * for consistency's sake.
 */
void gpio_init(void);

/*
 * Set a GPIO function for GPIO pin number `pin`. Settings for other pins
 * should be unchanged.
 *
 * @param pin      the GPIO pin number to initialize
 * @param function the GPIO function to set for the pin
 *
 * If `pin` or `function` is invalid, does nothing.
 *
 */
void gpio_set_function(unsigned int pin, unsigned int function);

/*
 * Get the GPIO function for GPIO pin number `pin`.
 *
 * @param pin the GPIO pin number to read the function of
 * @return    the current GPIO function of the specified pin
 *
 * If `pin` is invalid, returns GPIO_INVALID_REQUEST.
 *
 */
unsigned int gpio_get_function(unsigned int pin);

/*
 * Convenience functions for setting a pin to GPIO_FUNC_INPUT or
 * GPIO_FUNC_OUTPUT. The implementation calls `gpio_set_function`.
 *
 * @param pin the GPIO pin number to set the function of
 */
void gpio_set_input(unsigned int pin);
void gpio_set_output(unsigned int pin);

/*
 * Sets GPIO pin number `pin` to high (1) or low (0). This
 * function assumes the pin is already in output mode.
 * Settings for other pins should be unchanged.
 *
 * @param pin   the pin number to set or clear
 * @param val   1 if the pin should be set, 0 otherwise
 *
 * If `pin` is invalid, does nothing.
 *
 */
void gpio_write(unsigned int pin, unsigned int val);

/*
 * Get current value (1 for high, 0 for low) for GPIO pin number `pin`.
 *
 * @param pin the GPIO pin number to read the value of
 * @return    the value of the specified pin
 *
 * If `pin` is invalid, returns GPIO_INVALID_REQUEST.
 *
 */
unsigned int gpio_read(unsigned int pin);

/*
 * Copyright (c) 2018 Stanford University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the Stanford University nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL STANFORD
 * UNIVERSITY OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#endif
