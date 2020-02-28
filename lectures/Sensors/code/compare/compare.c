#include "gpio.h"

void main(void) 
{
    const int COMPARE = GPIO_PIN2;
    const int LED = GPIO_PIN20;

    gpio_init();
  
    gpio_set_input(COMPARE);
    gpio_set_output(LED);
  
    while (1)
        gpio_write(LED, gpio_read(COMPARE));
}
