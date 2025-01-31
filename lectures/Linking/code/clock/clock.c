/* Here is a comment */

#include "gpio.h"
#include "timer.h"

#define NUM_SEGMENTS 7
#define NUM_DIGITS 4
#define DELAY (1000/3)


static void blink(gpio_id_t pin) {
    gpio_write(pin, 1); // here is another comment
    timer_delay_ms(DELAY);
    gpio_write(pin, 0);
    timer_delay_ms(DELAY);
}

void main(void) {
    gpio_id_t segment[NUM_SEGMENTS] = {GPIO_PD17, GPIO_PB6, GPIO_PB12, GPIO_PB11, GPIO_PB10, GPIO_PE11, GPIO_PD13};
    gpio_id_t digit[NUM_DIGITS] = {GPIO_PB4, GPIO_PB3, GPIO_PB2, GPIO_PC0};

    for (int i = 0; i < NUM_SEGMENTS; i++) {
        gpio_set_output(segment[i]);
    }
    for (int i = 0; i < NUM_DIGITS; i++) {
        gpio_set_output(digit[i]);
    }

    while (1) {
        for (int i = 0; i < NUM_DIGITS; i++) {
            gpio_write(digit[i], 1);
            for (int j = 0; j < NUM_SEGMENTS; j++) {
                blink(segment[j]);
            }
            gpio_write(digit[i], 0);
        }
    }
}
