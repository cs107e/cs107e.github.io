#ifndef PWM_H__
#define PWM_H__

/*
    Module to interface with PWM hardware peripheral

    Author: Julie Zelenski
    Mon Nov 18 21:54:01 PST 2024
 */

#include "gpio.h"
#include <stdint.h>

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

void pwm_init(void);
void pwm_config_channel(pwm_channel_id_t ch, gpio_id_t pin, int freq, bool invert);

void pwm_set_duty(pwm_channel_id_t ch, int percentile); // percentile range 0-100%
void pwm_set_freq(pwm_channel_id_t ch, int freq);       // set freq, duty cycle 50% (square wave)

#endif