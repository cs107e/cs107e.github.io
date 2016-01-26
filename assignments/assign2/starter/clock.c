#include "timer.h"
#include "gpio.h"

/*
 * Returns the current system time in us.
 *
 * @return system time in microseconds
 */
#define RPI_SYSTIMER_BASE (*(int*)0x20003004)
unsigned int timer_get_time(void) {
	volatile unsigned int time = RPI_SYSTIMER_BASE;	// set time to system time
	return time;
}

/*
 * A simple busy loop that delays the program for `n` microseconds.
 *
 * @param n the number of microseconds to busy loop for
 */
void delay_us(unsigned int n) {
    unsigned int start = timer_get_time();
    while (timer_get_time() - start < n) { /* spin */ }
}

/*
 * Set GPIO pin number `pin` to the GPIO function `function`. Does not affect
 * other pins.
 *
 * @param pin the pin number to initialize
 * @param function the GPIO function to set for the pin
 */
#define GPIO_FSEL ((int*)0x20200000)
void gpio_set_function(unsigned int pin, unsigned int function) {
	int shift = (pin%10)*3;	// digits to shift to set pin as output
	if (pin < 10){	
		*(int *)0x20200000 += function << shift;	//set GPIO 1-9 as function
	} else if (pin >= 10 && pin < 20) {
		*(int *)0x20200004 += function << shift;	//set GPIO 11-19 as function
	} else {
		*(int *)0x20200008 += function << shift;	//set GPIO 20-29 as function
	}
}

/*
 * Sets GPIO pin number `pin` to the value `value`. The GPIO pin should be set
 * the output mode for this call to function correctly.
 *
 * @param pin the pin number to set or clear
 * @param value 1 if the pin should be set, 0 otherwise
 */
#define GPIO_SET ((int*)0x2020001C)
#define GPIO_CLR ((int*)0x20200028)
void gpio_write(unsigned int pin, unsigned int value) {
	int index = pin/32;
	int pos = pin%32;
	if (value)
		GPIO_SET[index] = 1 << pos;	// set GPIO to high
	else
		GPIO_CLR[index] = 1 << pos;	// set GPIO to low
}

void main(void) {
	for (int i = 10; i < 14; i++){	// set GPIO as output
		gpio_set_function(i, 1);
	}
	for (int i = 20; i < 27; i++){
		gpio_set_function(i, 1);
	}
	gpio_set_function(2, 0);	// set GPIO as input
	gpio_set_function(3, 0);
	int patterns[16];
	patterns[0] = 0b00111111;	// 0 pattern
	patterns[1] = 0b00000110;	// 1 pattern
	patterns[2] = 0b01011011;	// 2 pattern
	patterns[3] = 0b01001111;	// 3 pattern
	patterns[4] = 0b01100110;	// 4 pattern
	patterns[5] = 0b01101101;	// 5 pattern
	patterns[6] = 0b01111101;	// 6 pattern
	patterns[7] = 0b00000111;	// 7 pattern
	patterns[8] = 0b01111111;	// 8 pattern
	patterns[9] = 0b01101111;	// 9 pattern
	patterns[10] = 0b01110111;	// A pattern
	patterns[11] = 0b01111111;	// B pattern
	patterns[12] = 0b00111001;	// C pattern
	patterns[13] = 0b00111111;	// D pattern
	patterns[14] = 0b01111001;	// E pattern
	patterns[15] = 0b01110001;	// F pattern
	int adjustment = 0;	// Adjustment to be made by the two buttons 
	while(1){
		// turn off each digit
		for (int k = 10; k < 14; k++){
			gpio_write(k, 0);
		}
		// turn off each segment
		for (int k = 20; k < 27; k++){
			gpio_write(k, 0);
		}
		// Extension: adjust the time, using GPIO 16 & 19 to increase and decrease the time
		int input = (*(int *)0x20200034);	// retrieve input information
		int pin_2_on = input & (1<<2);
		int pin_3_on = input & (1<<3);
		if (pin_2_on){
			adjustment += 10;	// if + button is pressed 
		} 
		if (pin_3_on){
			adjustment -= 10;	// if - button is pressed
		}
		if (adjustment < 0){	// ensure adjustment doesn't go below 0
			adjustment = 0;	
		}
		unsigned int time_in_seconds = timer_get_time()/1000000 + adjustment;	// get adjusted time
		unsigned int time[6];	// array to hold the time
		time[0] = ((time_in_seconds)%60)%10;	// seconds
		time[1] = ((time_in_seconds)%60)/10;	// 10 seconds
		time[2] = ((time_in_seconds)/60)%60%10;	// minutes
		time[3] = ((time_in_seconds)/60)%60/10;	// 10 minutes
		time[4] = ((time_in_seconds)/3600)%24%10;	// hours
		time[5] = ((time_in_seconds)/3600)%24/10;	// 10 hours

		for (int i = 0; i < 4; i++){	// i = digit to turn on 
			int pattern_code = patterns[time[5-i]];	// find pattern for number to display
			gpio_write(10 + i, 1);	// writing to the digit
			for (int j = 0; j < 7; j++){
				if (pattern_code & (1 << j)){
					gpio_write(20 + j, 1);	// write to the segment
				} else {
					gpio_write(20 + j, 0);
				}
			}
		delay_us(2500);	
		gpio_write(10 + i, 0);	// clear  digit pattern
		}
	}
}
