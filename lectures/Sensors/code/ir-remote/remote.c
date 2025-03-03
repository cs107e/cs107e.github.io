/*
   http://irq5.io/2012/07/27/infrared-remote-control-protocols-part-1/

   Apple IR remote uses pulse length encoding

		Signal	off (µs)	on (µs)
		leader	9000		4500
		0 bit	560			560
		1 bit	560			1680

	align on leader (9000-off followed by 4500-on)
	then start pulling bits.
	long on is stop bit

	tsop 34838 IR sensor
 */

#include "gpio.h"
#include "gpio_extra.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

static const gpio_id_t IR_GPIO = GPIO_PB4; // pinout Signal-Gnd-Vcc

// read until value changes or 10ms timeout, whichever comes first
// return usec of time period val was held (-1 if timeout)
static int stable_read(gpio_id_t pin, int val) {
	unsigned long start_ticks = timer_get_ticks();
	unsigned long too_long = start_ticks + 10000*TICKS_PER_USEC;
	while (gpio_read(pin) == val) {
		if (timer_get_ticks() > too_long) {
			return -1;
		}
	}
	return (timer_get_ticks() - start_ticks)/TICKS_PER_USEC;
}

static inline int in_tolerance(int actual, int expected_len) {
	int eps = (expected_len*.25);
	return (actual >= expected_len - eps) && (actual <= expected_len + eps);
}

static int validate_bit(int off_time, int on_time) {
	const int PULSE_USEC = 560;
	const int zero_length = PULSE_USEC, one_length = PULSE_USEC*3;

	if (in_tolerance(off_time, PULSE_USEC) && in_tolerance(on_time, zero_length)) return 0;
	if (in_tolerance(off_time, PULSE_USEC) && in_tolerance(on_time, one_length)) return 1;

	printf("cannot decode: off=%d, on=%d (expected %d then %d or %d)\n", off_time, on_time, PULSE_USEC, zero_length, one_length);
	return -1;
}

static int expect(uint8_t bit, int expected_len) {
	int time = stable_read(IR_GPIO, bit);
	return time != -1 && in_tolerance(time, expected_len);
}

static uint32_t read_packet(void) {
	while (1) {
		// wait until aligned on leader
		while (!expect(0, 9000) || !expect(1, 4500));

	   	// bits sent order msb -> lsb
		uint32_t val = 0;
		while (1) {
			int on_time, off_time;
			if ((off_time = stable_read(IR_GPIO, 0)) == -1) {
				break;  // restart
			}
			// extra long on_time is stop bit
			if ((on_time = stable_read(IR_GPIO, 1)) == -1)
				return val;
			int bit = validate_bit(off_time, on_time);
			if (bit == -1) break; // restart
			val = (val << 1) | bit;
		}
	}
}

static const char *decode(uint32_t val, uint8_t *p_remote) {
	*p_remote = val & 0xff;
	uint8_t command = ((val >> 12) & 0x7);
	uint16_t apple = (val >> 16) & 0xfff;

	if (apple == 0x7e1) {
		switch (command) {
		  	case 0x1: return "<<";
		  	case 0x2: return "play/pause";
		  	case 0x3: return "-";
		  	case 0x4: return "menu";
			case 0x5: return "+";
			case 0x6: return ">>";
		}
	}
	return NULL;
}

void main(void) {
    timer_init();
    gpio_init();
	uart_init();

  	gpio_set_input(IR_GPIO);
  	gpio_set_pulldown(IR_GPIO);

  	printf("Starting program %s\n\n", __FILE__);
  	printf("Waiting on IR events from Apple remote...\n");
 	uint8_t last_id = 0;
 	while (1) {
  		uint8_t remote_id;
		uint32_t v = read_packet();
		const char *key = decode(v, &remote_id);
		if (key) {
			if (remote_id != last_id) {
				last_id = remote_id;
				printf("\tremote id now %x\n", remote_id);
			}
			printf("[%s]\n", key);
		} else {
			printf("unable to decode packet %x\n", v);
		}
	}
	printf("\nExiting program %s!\n", __FILE__);
}
