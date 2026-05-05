/* File: dc.c
 * ----------
 * L9110 DC motor driver attached to tiny propeller blade
 *
 * https://rydepier.wordpress.com/2016/05/30/l9110-fan-motor-keyes-board/
 */
#include "gpio.h"
#include "pwm.h"
#include "timer.h"

void main(void) {
    gpio_id_t pin = GPIO_PB6;
    pwm_channel_id_t ch = PWM1;

    pwm_init();
    pwm_config_channel(ch, pin, 80, PWM_ACTIVE_HIGH);

    for (int i = 0; i < 2; i++) {
        pwm_set_duty(ch, 20);   // slow-ish
        timer_delay(5);
        pwm_set_duty(ch, 40);   // 2x speed
        timer_delay(2);
        pwm_set_duty(ch, 0);    // off
        timer_delay(1);
    }
}
