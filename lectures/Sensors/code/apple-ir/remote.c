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
#include "gpio_extra.h"
#include "uart.h"
#include "printf.h"

static const gpio_id_t clk = GPIO_PB4;

static const char *apple_key(unsigned int val) {
	switch (val) {
		case 0x77e150b3: return "+";
		case 0x77e130b3: return "-";
		case 0x77e160b3: return ">>";
		case 0x77e190b3: return "<<";
		case 0x77e1a0b3: return "play/pause";
		case 0x77e1c0b3: return "menu";
		default: return "invalid decoding";
	}
}

// return usec of time period val was held (-1 on timeout)
static int stable_read(gpio_id_t pin, int val) {
	unsigned long start_ticks = timer_get_ticks();
	unsigned long too_long = start_ticks + 10000*TICKS_PER_USEC;
	while (gpio_read(pin) == val) {
		if (timer_get_ticks() > too_long)
			return -1;
	}
	return (timer_get_ticks() - start_ticks)/TICKS_PER_USEC;
}

static inline int within(int actual, int expected_len, int eps) {
	return (actual >= expected_len - eps) && (actual <= expected_len + eps);
}

static int validate_bit(int on_time, int off_time) {
	const int one_tick = 600, eps = 100;
	const int zero_length = one_tick, one_length = one_tick*3;

	if (within(on_time, one_tick, eps)) {
		if (within(off_time, zero_length, eps)) return 0;
		if (within(off_time, one_length, eps*3)) return 1;
	}
	printf("invalid?  on time=%d, off time =%d (expected on_time %d and off time %d or %d\n", on_time, off_time, one_tick, zero_length, one_length);
	return -1;
}

static int expect(unsigned int bit, unsigned int expected_len, unsigned int epsilon) {
	int time = stable_read(clk, bit);
	return time != -1 && within(time, expected_len, epsilon);
}

static unsigned int read_packet(void) {
	while (1) {
		// wait until aligned on the leader
		while(!expect(0, 9000, 500)
		|| !expect(1, 4500, 500)
		|| !expect(0, 600, 100)
		|| !expect(1, 600, 100))
			;

		unsigned int val = 0;
		while (1) {
			int on_time, off_time;
			if ((off_time = stable_read(clk, 0)) == -1) {
				break;
			}
			// stop bit is just 0 and a long on-signal.
			if ((on_time = stable_read(clk, 1)) == -1)
				return val;
			int bit = validate_bit(off_time, on_time);
			if (bit == -1) break;
			val = (val << 1) | bit;
		}
	}
}

void main(void) {
    timer_init();
    gpio_init();
	uart_init();

  	gpio_set_input(clk);
  	gpio_set_pulldown(clk);

  	printf("Waiting on IR events from Apple remote...\n");
  	while (1) {
		unsigned int v = read_packet();
		printf("hit apple remote key: %s\n", apple_key(v));
	}
}
