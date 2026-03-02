/*
 * hall effect sensor 3144
 *
 * looking down from top of pyramid = power, ground, signal
 * turn on internal pullup so don't need external resistor
 *
 * http://www.raspberrypi-spy.co.uk/2015/09/how-to-use-a-hall-effect-sensor-with-the-raspberry-pi/
 *
 * The output of these devices (pin 3) switches low when the magnetic
 * field at the Hall element exceeds the operate point threshold (BOP). At
 * this point, the output voltage is VOUT(SAT). When the magnetic field
 * is reduced to below the release point threshold (BRP), the device
 * output goes high. The difference in the magnetic operate and release
 * points is called the hysteresis (Bhys) of the device. This built-in
 * hysteresis allows clean switching of the output even in the presence
 * of external mechanical vibration and electrical noise.
 */

#include "gpio.h"
#include "gpio_extra.h"
#include "uart.h"
#include "printf.h"

void main(void) {
    gpio_init();
    uart_init();

    gpio_id_t pin = GPIO_PB3;
  	gpio_set_input(pin);
  	gpio_set_pullup(pin);

    int last = -1;
	while(1) {
        int cur = gpio_read(pin);
        if (cur != last) {
            printf("magnet %s\n", cur == 0 ? "detected" : "out of range");
            last = cur;
        }
	}
}
