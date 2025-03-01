---
permalink: /project/components
title: 'Project components and sensors'
toc: true
---

Your parts kit has some components you might repurpose (LEDs, resistors, transistors, buttons), and there are wide variety of additional components and sensors that you may want to consider incorporating into your project.

An excellent reference book for exploring sensors is "Make: Encyclopedia of Electronic Components Volume 3: Sensors" by Charles Platt and Fredrik Jansson.

![image of book](images/SensorsBook.jpg){: .zoom .w-50}
You can access the online version via Stanford Libraries Safari Books portal <https://searchworks.stanford.edu/view/13211489>.
 We also have one shared physical copy in the lab room. (Please do not remove from lab so that all may use it.)

Below is a partial list of components and sensors that have been successfully used in previous projects (Mango, and previously Raspberry Pi).

### Digital buttons/switches
* Buttons (press, snap-action, toggle, latching, dip, ...)
* Keypads
* Digital joystick
* Rotary encoder
* Tilt/vibration
* Capacitive touch
* Digital Hall effect

### Analog inputs
The Mango Pi GPIO pins are digital, reading an analog value requres interfacing with an analog to digital converter (ADC), such as the MCP3008.

* ADCs (analog-to-digital converters)
    - [MCP3008](https://www.digikey.com/product-detail/en/microchip-technology/MCP3008-I-P/MCP3008-I-P-ND/319422), 8-channel 10-bit SPI protocol
    - ADS 1155, 4-channel 16-bit I2C protocol
* Potentiometers in various forms (rotary, sliding, ribbon)
* Photoresistor
* Analog joystick
* Analog Hall effect
* Piezo vibration
* Force detection, strain gauge
* Temperature

### Optical/light

* IR Distance sensor (voltage proportional to distance)
* Phototransistor
* Passive infrared motion sensors
    - HC_SR501
* IR emitter/detector
    - [TSOP 43838 38 KHz](https://www.digikey.com/product-detail/en/vishay-semiconductor-opto-division/TSOP34838/751-1386-5-ND/1768188)
* RGB LEDs
* Laser emitter/detector
* Photointerruptor

### Sound/music

* Active and passive buzzers
* Electret microphones
* [HC-SR04 Ultrasonic Sensor/Distance Measuring modules](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) (time of flight)

### I2C

* Time-of-flight distance sensors [VL6180X](https://www.pololu.com/product/2489), [VL53L0X](https://www.pololu.com/product/2490)
* MMUs (accelerometer/gyro/altimeter/compass) GY-521, LSM6DS3

### SPI

* MCP3008 Analog-to-Digital convertor
* Arducam 2-Megapixel Camera OV2640 (requries both i2c and spi)
* Shift register HC595 SIPO, HC165 PISO


## Component suppliers

Here are some good vendors for electronic components.

* [Jameco](https://www.jameco.com/) has a great selection of electronic parts. They are also located in Burlingame near SFO.
If you place an online order, parts will generally arrive the next day via US mail. If you have a car, you can also drive there and pick up parts same day at the Will Call desk.

* [Pololu](https://www.pololu.com/) has a wide selection of parts for robotics, including sensors and motor drivers. They also document their parts very well.

* [Adafruit](https://www.adafruit.com/) Limor Fried has founded a company that caters to makers of all types. Besides building great products, they also provide extensive tutorials and learning guides on their web site.

* [Sparkfun](https://www.sparkfun.com/) is another great source for hobbyist electronics.

* Visit [lab64](https://ee.stanford.edu/news/lab64), the EE department's Maker Space located in the Packard building. This workspace has a variety of tools (oscilloscope, soldering stations, 3-D printer, laser cutter) and a store stocked with basic electronic parts such as transistors, op-amps, etc.

We also have a small lending library of some common components/sensors that we can loan for you to try out before ordering your own.
