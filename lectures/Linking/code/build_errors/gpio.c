#include "gpio.h"
#include <stddef.h>

void gpio_init(void) {
}

void gpio_set_input(gpio_id_t pin) {
    gpio_set_function(pin, GPIO_FN_INPUT);
}

void gpio_set_output(gpio_id_t pin) {
    gpio_set_function(pin, GPIO_FN_OUTPUT);
}

void gpio_set_function(gpio_id_t pin, unsigned int function) {
   /***** TODO: Your code goes here *****/
}

unsigned int gpio_get_function(gpio_id_t pin) {
    /***** TODO: Your code goes here *****/
    return 0;
}

void gpio_write(gpio_id_t pin, int value) {
    /***** TODO: Your code goes here *****/
}

int gpio_read(gpio_id_t pin) {
    return 0;
}

bool gpio_id_is_valid(gpio_id_t pin) {
    /***** TODO: Your code goes here *****/
    return false;
}