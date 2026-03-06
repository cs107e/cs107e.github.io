/* File: servo.c
 * -------------
 * SG90 MicroServo driven using PWM
 *
 * https://components101.com/motors/servo-motor-basics-pinout-datasheet
 */
#include "gpio.h"
#include "pwm.h"
#include "timer.h"


void main(void) {
    gpio_id_t pin = GPIO_PB1;
    pwm_channel_id_t ch = PWM4;

    pwm_init();

    int freq = 50; // 50 Hz
    int period_us = 1000*1000/freq;
    pwm_config_channel(ch, pin, freq, PWM_ACTIVE_HIGH);

    for (int i = 0; i < 4; i++) {
        pwm_set_duty(ch, 1500*100/period_us); // 1500 us => move to neutral position (and hold)
        timer_delay(1);
        pwm_set_duty(ch, 800*100/period_us);  // CCW min
        timer_delay_ms(500);
        pwm_set_duty(ch, 2500*100/period_us); // CW max
        timer_delay_ms(500);
    }
}
