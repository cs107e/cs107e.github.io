#include "gpio.h"
#include "timer.h"

const int pin = 20;

void blink(void)
{
    gpio_write(pin, 1);
    delay(1);
    gpio_write(pin, 0);
    delay(1);
}

void notmain(void)
{
    gpio_set_function(pin, GPIO_FUNC_OUTPUT);
    while (1)
        blink();
}
