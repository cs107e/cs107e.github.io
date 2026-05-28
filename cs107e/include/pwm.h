#pragma once
/*
 * File: pwm.h
 *
 * Module to interface with PWM hardware peripheral.
 *
 * This module is a proof of concept to demonstrate simple use
 * of the pwm hardware peripheral. The hardware supports many
 * additional features (paired channels, pulse train, and so on)
 * that this module does not provide. If you need fancier
 * functionality, take the code from this module as starting point
 * and upgrade it to do more!
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "gpio.h"
#include <stdint.h>

/*
 * Type: `pwm_channel_id_t`
 *
 * `pwm_channel_id_t` is an enumerated type for the 8 available pwm channels.
 */
typedef enum {
    PWM0 = 0,
    PWM1,
    PWM2,
    PWM3,
    PWM4,
    PWM5,
    PWM6,
    PWM7
} pwm_channel_id_t;

/*
 * Type: `pwm_active_state_t`
 *
 * `pwm_active_state_t` is an enumerated type for whether active low or active high.
 */
typedef enum {
    PWM_ACTIVE_LOW = 0,
    PWM_ACTIVE_HIGH = 1
} pwm_active_state_t;

/*
 * `pwm_init` : Required initialization for pwm module
 *
 * Initializes the pwm module. Must run before using the pwm functionality.
 *
 * Only need to call `pwm_init` once -- subsequent calls reinitialize the module.
 */
void pwm_init(void);

/*
 * `pwm_config_channel` : Config pwm channel
 *
 * Config the pwm channel to specified gpio, frequency, and active state.
 * The channel is enabled and initial duty cycle is set to 0 (off).
 *
 * @param ch        pwm channel
 * @param pin       gpio pin to direct pwm output
 * @param freq      frequency in Hz (cycles per second)
 * @param state     either PWM_ACTIVE_LOW or PWM_ACTIVE_HIGH
 */
void pwm_config_channel(pwm_channel_id_t ch, gpio_id_t pin, int freq, pwm_active_state_t state);

/*
 * `pwm_set_duty` : Set duty cycle of pwm channel
 *
 * Change duty cycle (i.e. percent of cycle in active state) to the specified value.
 *
 * @param ch        pwm channel
 * @param percent   value in range 0 (full inactive, off) to 100 (full active, on)
 */
void pwm_set_duty(pwm_channel_id_t ch, int percent); // range 0-100%

/*
 * `pwm_set_freq` : Set frequency of pwm channel
 *
 * Change frequency to the specified value and set duty cycle to 50%
 * This will output a square wave at specified frequency
 *
 * @param ch        pwm channel
 * @param freq      value in Hz (cycles per second)
 */
void pwm_set_freq(pwm_channel_id_t ch, int freq);
