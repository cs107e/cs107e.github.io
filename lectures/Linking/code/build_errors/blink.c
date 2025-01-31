#include "gpio.h"
#include "timer.h"

#define DELAY (1000/3)

void main(void) {
    gpio_id_t pin = GPIO_PD18;

    gpio_init();
    gpio_set_output(pin);

    while (1) {
        gpio_write(pin, 1);
        timer_delay_ms(DELAY);
        gpio_write(pin, 0);
        timer_delay_ms(DELAY);
    }
}
