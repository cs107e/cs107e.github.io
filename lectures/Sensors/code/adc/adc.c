#include "timer.h"
#include "gpio.h"
#include "gpioextra.h"
#include "uart.h"
#include "printf.h"
#include "assert.h"

const unsigned int signal = GPIO_PIN2;
const unsigned int discharge = GPIO_PIN3;

unsigned int get_charge_time(void) 
{
    // discharge the capacitor
    gpio_write(discharge, 1);
    timer_delay_ms(1);
    gpio_write(discharge, 0);

    // time how long it takes for the capacitor to charge.
	unsigned int start = timer_get_ticks();
	while(!gpio_read(signal))
		;
	unsigned int end = timer_get_ticks();

	return (end - start);
}

void main(void) 
{
    gpio_init();
	uart_init();

    gpio_set_output(discharge);
    gpio_set_input(signal);
    gpio_set_pullnone(signal);

  	while(1) {
		unsigned int time = get_charge_time();
		printf("time = %d\n", time);
	}
}
