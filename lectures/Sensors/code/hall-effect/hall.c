/*
 * hall effect sensor 3144.   
 *
 * http://www.allegromicro.com/~/media/Files/Datasheets/A3141-2-3-4-Datasheet.ashx?la=en
 *
 *
 * looking down from the top of the pyramid = power, ground, output (vout).
 * 	- vout to gpio2.
 *	- enable pullup so don't need a resistor.
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
#include "gpioextra.h"
#include "uart.h"
#include "printf.h"

void print_magnet(unsigned int vout)
{
    printf(vout ?  "magnet out of range\n" : "magnet detected\n" );
}

void main(void) {
	const unsigned vout= GPIO_PIN2;

    gpio_init();
    uart_init();

  	gpio_set_function(vout, GPIO_FUNC_INPUT);
  	gpio_set_pullup(vout);

    // vout is 1 when the magnet is out of range of the sensor
    print_magnet(1);
	while(1) {
  		while(gpio_read(vout) == 1) {} // wait for low
		print_magnet(0);
  		while(gpio_read(vout) == 0) {} // wait for high
		print_magnet(1);
	}
}
