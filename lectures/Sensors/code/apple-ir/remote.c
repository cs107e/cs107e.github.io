/*
 * tsop 34838 IR sensor.
 *
   Don't print while getting input.

   http://irq5.io/2012/07/27/infrared-remote-control-protocols-part-1/
   no signal = 1.

		Signal Part	off (µs)	on (µs)
			leader	9000		4500
			0 bit	560		560
			1 bit	560		1690
			stop	560		–

		we wait for a 9000off.
		then start pulling bits.
	
		not sure what stop is (timeout on on i believe).
 */
#include "timer.h"
#include "gpio.h"
#include "gpioextra.h"
#include "uart.h"
#include "printf.h"

const unsigned int clk = GPIO_PIN2;
const int timeout = 10000;
const unsigned int debounce = 3;

char *apple_key(unsigned int v) {
	switch(v) {
	case 0x77e150b3: return "+";
	case 0x77e130b3: return "-";
	case 0x77e160b3: return ">>";
	case 0x77e190b3: return "<<";
	case 0x77e1a0b3: return "play/pause";
	default: return "invalid decoding";
	}
}

// return usecs value was held.
unsigned int stable_read(unsigned int pin, unsigned int val, unsigned int timeout) {
	unsigned int clocks = 0;
	unsigned int time = timer_get_ticks();
retry:
	while(gpio_read(pin) == val) {
		if((timer_get_ticks() - time) > timeout)
			return timeout;
	}

	for(clocks = 0; clocks < debounce; clocks++) 
		if(gpio_read(pin) == val)
			goto retry;

	return timer_get_ticks() - time;
}

// don't want to use FP.
int within(unsigned int actual, unsigned int expected, unsigned int eps) { 
	if(actual > (expected + eps) || actual < (expected - eps))
		return 0;
	return 1;
}

unsigned int bit(unsigned int on, unsigned int off) { 
	const unsigned one_tick = 600, eps = 100;

	if(within(on, one_tick, eps)) {
		if(within(off, one_tick, eps))
			return 0;
		if(within(off, one_tick*3, eps*3))
			return 1;
		printf("invalid?   have off=%d, expected %d or %d\n", off, 
						one_tick, one_tick*3);
		return -1;	
	}
	printf("invalid?   have on=%d (off=%d), expected %d or %d\n", 
		on, off, one_tick, one_tick*3);
	return -1;
}

int expect(unsigned int bit,  unsigned int usec, unsigned int eps) {
	unsigned time = stable_read(clk, bit, timeout);
	return within(time, usec, eps);
}

unsigned int read_packet(void) {
	unsigned int on, off;

retry:
	// wait til we get aligned on the header.
	while(!expect(0, 9000, 500) 
	|| !expect(1, 4500, 500)
	|| !expect(0, 600, 100) 
	|| !expect(1, 600, 100)) 
		;

	unsigned int val = 0;
	while(1) {
		if((off = stable_read(clk, 0, timeout)) == timeout) {
			printf("invalid packet, timeout on off-read\n");
			goto retry;
		}

		// stop bit is just 0 and a long on-signal.
		if((on = stable_read(clk, 1, timeout)) == timeout) 
			return val;

		unsigned int b = bit(off, on);
		if(b == -1)
			goto retry;
		val = (val << 1) | b;
	}
}

void main(void) {
    timer_init();
    gpio_init();
	uart_init();

  	gpio_set_function(clk, GPIO_FUNC_INPUT);
  	gpio_set_pulldown(clk);

  	for(int i = 0;  i < 20; i++) {
		unsigned int v = read_packet();
		printf("hit apple remote key: %s\n", apple_key(v));
	}
}
