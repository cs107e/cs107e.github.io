#include "gpio.h"

volatile unsigned int *GPIO_BASE = (volatile unsigned int *)0x02000000;

void gpio_init(void) {
}

void gpio_set_input(gpio_id_t pin) {
    gpio_set_function(pin, GPIO_FN_INPUT);
}

void gpio_set_output(gpio_id_t pin) {
    gpio_set_function(pin, GPIO_FN_OUTPUT);
}

void gpio_set_function(gpio_id_t pin, unsigned int function) {
}

unsigned int gpio_get_function(gpio_id_t pin) {
    return 0;
}

void gpio_write(gpio_id_t pin, int value) {
}

int gpio_read(gpio_id_t pin) {
    return 0;
}

bool gpio_id_is_valid(gpio_id_t pin) {
    return false;
}
