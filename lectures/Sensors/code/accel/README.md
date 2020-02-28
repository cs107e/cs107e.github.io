MinIMU-9 v3 Gyro, Accelerometer, and Compass (L3GD20H and LSM303D Carrier)
https://www.pololu.com/product/2468

A minimum of four connections are necessary to use the MinIMU-9:
VIN, GND, SCL, and SDA. 
- SDA is GPIO_PIN2
- SCL is GPIO_PIN3

VIN should be connected to a 2.5 V to 5.5 V source GND to 0 volts,

SCL and SDA should be connected to an I²C bus operating at the same
logic level as VIN.

(Alternatively, if you are using the board with a 3.3 V system, you
can leave VIN disconnected and bypass the built-in regulator by
connecting 3.3 V directly to VDD.)

---- Dawson's notes ----

images/gpio.jpg

pin layout:
	images/lsm-pins.jpg

package:
	images/package.jpg
	~/class/master.2016.spr/staff/libpi/tests/i2c_tests

where do we go?
	p23
what are 
	CTRL9_XL
	CTRL_1_XL
	CTRL_10_C
	CTRL2_G

	p8.

	what are the units?
	what are the rest of these names?


where do we get the scale?
	[pulled from arduino code from manufacturer's site.]
