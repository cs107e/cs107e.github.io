/* File: adc.c
 * -----------
 * Analog to digital conversion using timing-based RC circuit
 */

#include "gpio.h"
#include "timer.h"
#include "printf.h"
#include "uart.h"

static unsigned int get_charge_time(gpio_id_t signal, gpio_id_t discharge) {
    gpio_write(discharge, 1);     // discharges capacitor
    timer_delay_ms(1);
    gpio_write(discharge, 0);

    // wait for capacitor to charge and measure how long it takes
	unsigned long start_ticks = timer_get_ticks();
	while(gpio_read(signal) == 0)
		;
	return (timer_get_ticks() - start_ticks)/TICKS_PER_USEC;
}

void main(void) {
    gpio_init();
	uart_init();
    printf("Starting program %s\n\n", __FILE__);
    printf("Analog to digital conversion using timing-based RC circuit\n");

    gpio_id_t signal = GPIO_PB3;
    gpio_id_t discharge = GPIO_PB4;
    gpio_set_output(discharge);
    gpio_set_input(signal);

  	while (1) {
		unsigned int time = get_charge_time(signal, discharge);
		printf("charge time = %d usecs\n", time);
	}
}
