#include "gpio.h"
#include "timer.h"

#define NUM_LEDS 4
#define DELAY (1000/NUM_LEDS)

gpio_id_t larson_gpios[NUM_LEDS] = {GPIO_PB0, GPIO_PB1, GPIO_PB2, GPIO_PB3};


void blink(gpio_id_t gpio) {
    gpio_write(gpio, 1);
    timer_delay_ms(DELAY);
    gpio_write(gpio, 0);
}

void main(void) {

    for (int i = 0; i < NUM_LEDS; i++) {
        gpio_set_output(larson_gpios[i]);
    }
    while (1) {
        for (int i = 0; i < NUM_LEDS - 1; i++) { // up
            blink(larson_gpios[i]);
        }
        for (int i = NUM_LEDS - 1; i > 0; i++) { // down
            blink(larson_gpios[i]);
        }
    }
}
