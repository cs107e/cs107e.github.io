#include "timer.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "uart.h"
#include "printf.h"

static const gpio_id_t SIGNAL = GPIO_PB3;
static const gpio_id_t DISCHARGE = GPIO_PB4;


static unsigned int get_charge_time(void) {
    // discharge the capacitor
    gpio_write(DISCHARGE, 1);
    timer_delay_ms(1);
    gpio_write(DISCHARGE, 0);

    // time how long it takes for the capacitor to charge.
	unsigned long start_ticks = timer_get_ticks();
	while(!gpio_read(SIGNAL))
		;
	return (timer_get_ticks() - start_ticks)/TICKS_PER_USEC;
}

void main(void) {
    gpio_init();
	uart_init();

    gpio_set_output(DISCHARGE);
    gpio_set_input(SIGNAL);
    gpio_set_pullnone(SIGNAL);

  	while (1) {
		unsigned int time = get_charge_time();
		printf("charge time = %d usecs\n", time);
	}
}
