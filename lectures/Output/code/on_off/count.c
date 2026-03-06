// SIPO shift register SN74HC595N
// used  to control eight outputs
// only requires 3 gpio pins
// Use 8 leds to display binary counter

#include "gpio.h"
#include "printf.h"
#include "timer.h"
#include <stdint.h>

static void pulse(gpio_id_t pin, int us) {
    gpio_write(pin, 1);
    timer_delay_us(us);
    gpio_write(pin, 0);
}

static void output_byte(uint8_t byte, gpio_id_t latch, gpio_id_t shift, gpio_id_t data) {
   gpio_write(latch, 0); // latch before write
   for (int i = 7; i >= 0; i--) { // bit order is msb -> lsb
        gpio_write(data, (byte >> i) & 0x1); // write bit
        pulse(shift, 1); // shift one
    }
    pulse(latch, 1); // unlatch
}

void main(void)  {
    gpio_init();
    timer_init();

    gpio_id_t latch = GPIO_PB4, shift = GPIO_PB3, data = GPIO_PB2;
    gpio_set_output(latch);
    gpio_set_output(shift);
    gpio_set_output(data);

    for (int i = 0; i <= 255; i++) {
        output_byte(i, latch, shift, data);
        timer_delay_ms(60);
    }
}
