#ifndef GPIO_REF
#define GPIO_REF
#include "gpio.h"
void ref_gpio_init(void);
void ref_gpio_set_function(unsigned int pin, unsigned int function);
unsigned int ref_gpio_get_function(unsigned int pin);
void ref_gpio_set_input(unsigned int pin);
void ref_gpio_set_output(unsigned int pin);
void ref_gpio_write(unsigned int pin, unsigned int val);
unsigned int ref_gpio_read(unsigned int pin);
#endif
