/* File: blink_gpio.c
 * ------------------
 *  blink rewritten to use gpio module. Hooray for abstraction!
 */
#include "gpio.h"
#include "timer.h"

#define SECOND 1000

const gpio_id_t pin = GPIO_PB0;

void main(void) {
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
