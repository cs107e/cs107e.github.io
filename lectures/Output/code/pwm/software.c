/* File: software.c
 * ----------------
 * Simple example of poor woman's software PWM (pulse width modulation)
 *
 * Fade in/out of LED
 */

#include "gpio.h"
#include "timer.h"

static const gpio_id_t LED = GPIO_PB4;

static void on_fractional(gpio_id_t pin, float frac, int total_ms) {
    // turn on for fraction of period, off for rest
    // continue cycle for time total_ms
    const int frequency = 440; // Hz
    const int usec_per_cycle = (1000*1000)/frequency;
    int on_time = frac * usec_per_cycle;
    int off_time = usec_per_cycle - on_time;
    int ncycles = total_ms*1000/usec_per_cycle;
    for (int i = 0; i < ncycles; i++) {
        gpio_write(pin, 1);
        timer_delay_us(on_time);
        gpio_write(pin, 0);
        timer_delay_us(off_time);
    }
}

void main(void) {
    gpio_set_output(LED);

    while (1) {
        for (float f = 0; f <= 1.0; f += .01) {
            on_fractional(LED, f, 10); // 1 sec ramp up
        }
        for (float f = 1.0; f >= 0; f -= .01) {
            on_fractional(LED, f, 50); // 5 sec ramp down
       }
   }
}
