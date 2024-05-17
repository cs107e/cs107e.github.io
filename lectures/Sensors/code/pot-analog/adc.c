/* File: adc.c
 * -----------
 * Analog to digital conversion using timing-based RC circuit
 */

#include "gpio.h"
#include "timer.h"
#include "printf.h"
#include "uart.h"

static const gpio_id_t SIGNAL = GPIO_PB3;
static const gpio_id_t DISCHARGE = GPIO_PB4;

static unsigned int get_charge_time(void) {
    gpio_write(DISCHARGE, 1);     // discharge capacitor
    timer_delay_ms(1);
    gpio_write(DISCHARGE, 0);

    // time how long it takes for capacitor to charge
	unsigned long start_ticks = timer_get_ticks();
	while(gpio_read(SIGNAL) == 0)
		;
	return (timer_get_ticks() - start_ticks)/TICKS_PER_USEC;
}

void main(void) {
    gpio_init();
	uart_init();
    printf("Starting program %s\n\n", __FILE__);
    printf("Analog to digital conversion using timing-based RC circuit\n");

    gpio_set_output(DISCHARGE);
    gpio_set_input(SIGNAL);
  	while (1) {
		unsigned int time = get_charge_time();
		printf("charge time = %d usecs\n", time);
	}
}
