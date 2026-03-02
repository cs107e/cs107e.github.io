/*
   http://irq5.io/2012/07/27/infrared-remote-control-protocols-part-1/

   Apple IR remote uses pulse-distance modulation
   Carrier frequency is 38Khz, pulse width 560 us
   Bit encoding:
   	0: one pulse-width high, one pulse-width low
   	1: one pulse-width hight, three pulse-width low

   32-bit packet consists of 16-bit Apple code (0xEE 0x87), a command byte, and a remote ID byte

	De-modulated signal is encoded in timing of present+absent
					present 	absent    (µs)
		leader		9000		4500
		0 bit		560			560
		1 bit		560			1680

	align on leader (9000-present followed by 4500-absent)
	then packet of 32 bits (zero-bit = 560-present 560 or 1680 absent)
	long on is stop bit

	tsop 34838 IR sensor, active low output
		outputs low (0V) when detect presence of modulated IR signal (e.g. 38 kHz carrier)
		outputs high (Vcc) when signal not present
 */

#include "gpio.h"
#include "gpio_extra.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

enum state_t {PRESENT = 0, ABSENT = 1, NOT_VALID = -1};

// read until value changes or 10ms timeout, whichever comes first
// return usec of period val was held (return NOT_VALID if timeout, long absence could be stop bit)
static int stable_read(gpio_id_t pin, int val) {
	unsigned long start_ticks = timer_get_ticks();
	unsigned long too_long = start_ticks + 10000*TICKS_PER_USEC;
	while (gpio_read(pin) == val) {
		if (timer_get_ticks() > too_long) {
			return NOT_VALID;
		}
	}
	return (timer_get_ticks() - start_ticks)/TICKS_PER_USEC;
}

static inline bool in_tolerance(int actual, int expected_len) {
	if (actual == NOT_VALID) return false;
	int eps = (expected_len*.25); // tolerance 25% (cheap oscillators)
	return (actual >= expected_len - eps) && (actual <= expected_len + eps);
}

static int validate_bit(int present_us, int absent_us) {
	const int PULSE_USEC = 560;
	const int len_zero = PULSE_USEC, len_one = PULSE_USEC*3;

	if (in_tolerance(present_us, PULSE_USEC) && in_tolerance(absent_us, len_zero)) return 0;
	if (in_tolerance(present_us, PULSE_USEC) && in_tolerance(absent_us, len_one)) return 1;

	printf("cannot decode: present=%d, absent=%d (expected %d then %d or %d)\n", present_us, absent_us, PULSE_USEC, len_zero, len_one);
	return NOT_VALID;
}

static int expect(gpio_id_t pin, uint8_t bit, int expected_len) {
	int time = stable_read(pin, bit);
	return in_tolerance(time, expected_len);
}

static uint32_t read_packet(gpio_id_t pin) {
	while (1) {
		// wait until aligned on leader
		while (!expect(pin, PRESENT, 9000) || !expect(pin, ABSENT, 4500));

	   	// bit order within byte lsb -> msb
		uint32_t val = 0;
		int pos = 0;
		while (1) {
			int present_us, absent_us;
			if ((present_us = stable_read(pin, PRESENT)) == NOT_VALID) {
				break;  // restart
			}
			// extra long absence is stop bit
			if ((absent_us = stable_read(pin, ABSENT)) == NOT_VALID)
				return val;
			int bit = validate_bit(present_us, absent_us);
			if (bit == NOT_VALID) break; // restart
			val |= (bit << pos++);
		}
	}
}

static const char *decode_packet(uint32_t val, uint8_t *p_remote_id) {
	// 32-bit packet consists of 2-byte Apple code (0xee 0x87), command byte, and remote ID byte

	*p_remote_id = (val >> 24) & 0xff;
	uint8_t command = (val >> 17) & 0x7f; // 7-bits cmd, 1-bit parity (not checked)
	uint16_t apple = val & 0xffff;
	if (apple == 0x87ee) {
		switch (command) {
			case 0x1: return "menu";
			case 0x2: return "play/pause";
			case 0x3: return ">>";
		  	case 0x4: return "<<";
			case 0x5: return "+";
			case 0x6: return "-";
		}
	}
	return NULL;
}

void main(void) {
    timer_init();
    gpio_init();
	uart_init();
	gpio_id_t IR_GPIO = GPIO_PB4; // pinout Signal-Gnd-Vcc

  	gpio_set_input(IR_GPIO);
  	gpio_set_pullup(IR_GPIO);

  	printf("Starting program %s\n\n", __FILE__);
  	printf("Waiting on IR events from Apple remote...\n");
 	uint8_t last_id = 0;
 	while (1) {
  		uint8_t remote_id;
		uint32_t v = read_packet(IR_GPIO);
		const char *key_press = decode_packet(v, &remote_id);
		if (key_press) {
			printf("\t\tpacket=%32b\n", v);
			printf("[%s]", key_press);
			if (remote_id != last_id) {
				last_id = remote_id;
				printf("\tremote id now %x", remote_id);
			}
			printf("\n");
		} else {
			printf("unable to decode packet %32b %08x\n", v, v);
		}
	}
	printf("\nExiting program %s!\n", __FILE__);
}
