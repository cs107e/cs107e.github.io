#include "gpio.h"
#include "timer.h"

#define SECOND 1000

const int pin = GPIO_PIN20;

void main(void)
{
    timer_init();
	gpio_init();
	gpio_set_output(pin);
	
	while (1) {
		gpio_write(pin, 1);
		timer_delay_ms(SECOND);
		gpio_write(pin, 0);
		timer_delay_ms(SECOND);
	}
}
