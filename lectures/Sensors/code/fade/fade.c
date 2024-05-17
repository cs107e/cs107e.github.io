/* File: fade.c
 * ------------
 * Simple example of poor woman's soft PWM (pulse width modulation)
 * (more robust option would use D-1 hardware PWM peripheral !)
 */

#include "gpio.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

static const gpio_id_t LED = GPIO_PB4;

static void set_level(gpio_id_t pin, int ms, int percent_on) {
    // show for, on for x% of period, off for rest
    // 100hz, i.e. each cycle 10 ms = 10K usec
    int usec_per_cycle = 10*1000;
    int on_time = (percent_on * usec_per_cycle)/100;
    int off_time = usec_per_cycle - on_time;
    for (int i = 0; i < ms/10; i++) {
        gpio_write(pin, 1);
        timer_delay_us(on_time);
        gpio_write(pin, 0);
        timer_delay_us(off_time);
    }
}

void main(void) {
    uart_init();
    gpio_set_output(LED);

    printf("Starting program %s\n", __FILE__);
    while (1) {
        for (int percent = 0; percent <= 100; percent++) {
            set_level(LED, 15, percent); // 1.5 sec inhale
        }
        for (int percent = 100; percent >= 0; percent--) {
            set_level(LED, 30, percent); // 3 sec exhale
       }
   }
}
