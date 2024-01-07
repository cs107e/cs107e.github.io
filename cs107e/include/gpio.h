#ifndef GPIO_H
#define GPIO_H

/*
 * Functions for controlling Mango Pi GPIO.
 *
 * Students implement this module in assignment 2.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>

#define GPIO_INVALID_REQUEST  -1  // return value for invalid request

/*
 * Type: `gpio_id_t`
 *
 * `gpio_id_t` is an enumerated type used to refer to a gpio pin.
 * A C enumeration is effectively an (unsigned) int.
 * See the enumeration below for symbolic names/constants for
 * the valid ids on the Mango Pi.
 */
typedef enum _mango_ids gpio_id_t;

/*
 * `gpio_init`
 *
 * Initialize the GPIO code module. For assignment 2, this does nothing.
 * However, all peripheral modules require an init, so it is included
 * for consistency's sake.
 */
void gpio_init(void);

/*
 * `gpio_id_is_valid`
 *
 * Returns true if `pin` refers to a valid GPIO pin id, false otherwse.
 * See enumeration `gpio_id_t` below for list of valid ids.
 */
bool gpio_id_is_valid(gpio_id_t pin);

/*
 * `gpio_set_function`
 *
 * Set a GPIO function for GPIO with id `pin`. Settings for other pins
 * should be unchanged.
 *
 * @param pin       the id of the GPIO pin
 * @param function  the GPIO function to set for the pin
 *
 * If `pin` or `function` is not valid, does nothing.
 */
void gpio_set_function(gpio_id_t pin, unsigned int function);

/*
 * `gpio_get_function`
 *
 * Get the GPIO function for GPIO with id `pin`.
 *
 * @param pin the id of the GPIO pin
 * @return    the current GPIO function of the specified pin
 *
 * If `pin` is not valid, returns GPIO_INVALID_REQUEST.
 */
unsigned int gpio_get_function(gpio_id_t pin);

/*
 * `gpio_set_input`, `gpio_set_output`
 *
 * Convenience functions for setting a pin to GPIO_FN_INPUT or
 * GPIO_FN_OUTPUT. The implementation calls `gpio_set_function`.
 *
 * @param pin      the id of the GPIO pin to set function
 */
void gpio_set_input(gpio_id_t pin);
void gpio_set_output(gpio_id_t pin);

/*
 * `gpio_read`
 *
 * Read current value for GPIO with id `pin`.
 * Returns 1 if pin is high, 0 if low.
 *
 * @param pin   the id of the GPIO pin
 * @return      the value of the specified pin
 *
 * If `pin` is not valid, returns GPIO_INVALID_REQUEST.
 */
int gpio_read(gpio_id_t pin);

/*
 * `gpio_write`
 *
 * Set value of GPIO with id `pin` to high (1) or low (0).
 * This function assumes the pin is already in output mode.
 * Settings for other pins should be unchanged.
 *
 * @param pin   the id of the GPIO pin
 * @param val   1 to set pin high, 0 to set pin low
 *
 * If `pin` or `val` is not valid, does nothing.
 */
void gpio_write(gpio_id_t pin, int val);

/*
 * Enumeration of ids for GPIO pins on the Mango Pi
 *
 * Each GPIO pin has a symbolic id in the enumeration below.
 *
 * The GPIO pins are organized into six groups: PB PC PD PE PF PG
 * (These are called "ports" in the user manual)
 * The pins within a group are indexed by number starting from 0. Each 
 * group has a differing number of pins. For example, the PB group has 
 * 13 pins indexed from PB0 to PB12. The PC group has 8 pins PC0 to PC7.
 *
 * Values are assigned to each pin id in such a way to use a single hex constant
 * that mashes together the group and pin index as shown below
 *       constant 0xNnn  N = which group,  nn = pin index within group
 */
enum _mango_ids {
    GPIO_PB0 = 0x000, // start of B group, enum vals assigned in sequence from here
    GPIO_ID_FIRST = GPIO_PB0,
    GPIO_PB1,
    GPIO_PB2,
    GPIO_PB3,
    GPIO_PB4,
    GPIO_PB5,
    GPIO_PB6,
    GPIO_PB7,
    GPIO_PB8,
    GPIO_PB9,
    GPIO_PB10,
    GPIO_PB11,
    GPIO_PB12,
    GPIO_PB_LAST_INDEX = GPIO_PB12 - GPIO_PB0,
    GPIO_PC0 = 0x100, // start of C group
    GPIO_PC1,
    GPIO_PC2,
    GPIO_PC3,
    GPIO_PC4,
    GPIO_PC5,
    GPIO_PC6,
    GPIO_PC7,
    GPIO_PC_LAST_INDEX = GPIO_PC7 - GPIO_PC0,
    GPIO_PD0 = 0x200, // start of D group
    GPIO_PD1,
    GPIO_PD2,
    GPIO_PD3,
    GPIO_PD4,
    GPIO_PD5,
    GPIO_PD6,
    GPIO_PD7,
    GPIO_PD8,
    GPIO_PD9,
    GPIO_PD10,
    GPIO_PD11,
    GPIO_PD12,
    GPIO_PD13,
    GPIO_PD14,
    GPIO_PD15,
    GPIO_PD16,
    GPIO_PD17,
    GPIO_PD18,
    GPIO_PD19,
    GPIO_PD20,
    GPIO_PD21,
    GPIO_PD22,
    GPIO_PD_LAST_INDEX = GPIO_PD22 - GPIO_PD0,
    GPIO_PE0 = 0x300, // start of E group
    GPIO_PE1,
    GPIO_PE2,
    GPIO_PE3,
    GPIO_PE4,
    GPIO_PE5,
    GPIO_PE6,
    GPIO_PE7,
    GPIO_PE8,
    GPIO_PE9,
    GPIO_PE10,
    GPIO_PE11,
    GPIO_PE12,
    GPIO_PE13,
    GPIO_PE14,
    GPIO_PE15,
    GPIO_PE16,
    GPIO_PE17,
    GPIO_PE_LAST_INDEX = GPIO_PE17 - GPIO_PE0,
    GPIO_PF0 = 0x400, // start of F group
    GPIO_PF1,
    GPIO_PF2,
    GPIO_PF3,
    GPIO_PF4,
    GPIO_PF5,
    GPIO_PF6,
    GPIO_PF_LAST_INDEX = GPIO_PF6 - GPIO_PF0,
    GPIO_PG0 = 0x500, // start of G group
    GPIO_PG1,
    GPIO_PG2,
    GPIO_PG3,
    GPIO_PG4,
    GPIO_PG5,
    GPIO_PG6,
    GPIO_PG7,
    GPIO_PG8,
    GPIO_PG9,
    GPIO_PG10,
    GPIO_PG11,
    GPIO_PG12,
    GPIO_PG13,
    GPIO_PG14,
    GPIO_PG15,
    GPIO_PG16,
    GPIO_PG17,
    GPIO_PG18,
    GPIO_PG_LAST_INDEX = GPIO_PG18 - GPIO_PG0,
    GPIO_ID_LAST = GPIO_PG18,
};

#define GPIO_UART_TX GPIO_PB8
#define GPIO_UART_RX GPIO_PB9

/*
 * Enumeration for GPIO functions
 *
 * The enumerated values below establish symbolic names for each of the
 * available GPIO pin functions. Each pin function corresponds to
 * a particular "mode" of operation.  For example, setting a pin's
 * function to GPIO_FN_INPUT configures the pin to be used as an input.
 */
enum {
    GPIO_FN_INPUT  = 0,
    GPIO_FN_OUTPUT = 1,
    GPIO_FN_ALT2   = 2,
    GPIO_FN_ALT3   = 3,
    GPIO_FN_ALT4   = 4,
    GPIO_FN_ALT5   = 5,
    GPIO_FN_ALT6   = 6,
    GPIO_FN_ALT7   = 7,
    GPIO_FN_ALT8   = 8,
    // 9 -13 are reserved
    GPIO_FN_INTERRUPT = 14,
    GPIO_FN_DISABLED = 15,
};

#endif
