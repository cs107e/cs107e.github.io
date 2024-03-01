/*
 * HC-SR04 ultrasonic range sensor
 * Datasheet https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
 */
#include "timer.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "mango.h"
#include "uart.h"
#include "printf.h"

static int sense_distance(gpio_id_t trigger, gpio_id_t echo) {
	gpio_write(trigger, 1);
	timer_delay_us(10);    // pulse (set trigger hi for 10 us)
	gpio_write(trigger, 0);

    while (gpio_read(echo) != 1);
    unsigned long start_tick = timer_get_ticks();
	while (gpio_read(echo) == 1) ; // measure how long echo held high (width of pulse)
    unsigned long usecs = (timer_get_ticks() - start_tick)/TICKS_PER_USEC;

    // speed of sound in room temperature air 1132 ft/sec
    // (1 / (1132 ft/sec * 12in/ft / 10^6 usec/sec)) = 74 usec/inch
    // Note: Calculation is correct, to calibrate with real-world, empirical testing will be needed
    int inches = usecs / 74;
    // divide distance by 2 because timing to object and back
	return inches / 2;
}

void main(void) {
    const gpio_id_t echo = GPIO_PB4, trigger = GPIO_PB3;

    timer_init();
    gpio_init();
    uart_init();
  	gpio_set_output(trigger);
  	gpio_set_input(echo);
  	gpio_set_pulldown(echo);

  	while (1) {
		int inches = sense_distance(trigger, echo);
        printf("distance = %d inches\n", inches);
        timer_delay_ms(250);
	}
}
