/*
    https://hackaday.com/2010/09/02/using-an-nes-controller-on-an-android-phone/
    https://tresi.github.io/nes/

     NES uses a serialized polling mechanism to query the state of the buttons.
     Every 60 Hz, the NES sends a 12us high signal to the Latch pin, telling the controller
     to latch the state of of all buttons internally.
     Six microseconds later, the NES sends 8 high pulses on the Pulse pin, 12us per full cycle, 50% duty cycle.
*/

#include "gpio.h"
#include "gpio_extra.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

static void pulse_pin(gpio_id_t pin, int usec) {
    gpio_write(pin, 1);
    timer_delay_us(usec);
    gpio_write(pin, 0);
    timer_delay_us(6);
}

static uint8_t nes_read_all(gpio_id_t latch, gpio_id_t clock, gpio_id_t data) {
    uint8_t all = 0;
    pulse_pin(latch, 12);
    for (int i = 0; i < 8; i++) {
        int bit = !gpio_read(data); // active low, if pressed -> 0
        all |= bit << i;
        pulse_pin(clock, 6);
    }
    return all;
}

static void show_buttons(uint8_t state) {
    static uint8_t last;
    const char *labels = "AB#*NSWE";
    if (state != last) {
        uart_putchar('\r');
        for (int i = 0; i < 8; i++) {
            if  (state & (1 << i))
                printf("|%c", labels[i]);
            else
                printf("| ");
        }
        printf("|");
        last = state;
    }
}

void main(void) {
    timer_init();
    gpio_init();
	uart_init();
	gpio_id_t data = GPIO_PB0;    // brown
	gpio_id_t clock = GPIO_PD22;  // blue
    gpio_id_t latch = GPIO_PD21;  // yellow
	gpio_set_output(clock);
	gpio_set_output(latch);
	gpio_set_input(data);
	gpio_set_pullup(data);

  	printf("Starting program %s\n", __FILE__);
    printf("Read button state of SNES controller...\n\n");
 	while (1) {
 		uint8_t buttons = nes_read_all(latch, clock, data);
        show_buttons(buttons);
        timer_delay_ms(1000/60); // NES polling rate 60Hz
	}
	printf("\nExiting program %s!\n", __FILE__);
}
