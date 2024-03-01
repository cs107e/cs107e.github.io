#include "gpio.h"

void main(void) 
{
    const gpio_id_t COMPARE = GPIO_PB3;
    const gpio_id_t LED = GPIO_PB4;

    gpio_init();
  
    gpio_set_input(COMPARE);
    gpio_set_output(LED);
  
    while (1)
        gpio_write(LED, gpio_read(COMPARE));
}
