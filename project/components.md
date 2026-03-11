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
- Buttons (press, snap-action, toggle, latching, dip, ...)
- Keypad
- Digital joystick
- Rotary encoder
- Tilt/vibration/shock
- Capacitive touch
- Digital Hall effect

### Analog inputs
The Mango Pi GPIO pins are digital, reading an analog value requres interfacing with an analog to digital converter (ADC). The D1-H has ADC on-chip but no analog inputs are broken out on our board 🥲

- ADCs (analog-to-digital converters)
    - [mcp3008](https://www.digikey.com/product-detail/en/microchip-technology/MCP3008-I-P/MCP3008-I-P-ND/319422), 8-channel 10-bit, SPI protocol
    - [ADS 1115](https://www.adafruit.com/product/1085), 4-channel 16-bit, I2C protocol
- Potentiometers: rotary, sliding, ribbon
- Photoresistor
- Analog joystick
- Analog Hall effect
- Piezo vibration
- Force detection, strain gauge
- Temperature
- Flex sensor

### Optical/light

- IR distance sensor
- Phototransistor
- Passive infrared motion sensors [HC_SR501](https://a.co/d/0hGCCZqw)
- IR emitter/detector [TSOP 43838 38 KHz](https://www.digikey.com/product-detail/en/vishay-semiconductor-opto-division/TSOP34838/751-1386-5-ND/1768188)
- RGB LED
- Laser emitter/detector
- Photointerruptor

### Sound/music

- Active and passive buzzer
- Electret microphone
- Ultrasonic distance [HC-SR04](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) (time of flight)
- I2S digital audio
    - PCM5102A (DAC digital-to-analog output)
    - SPH0645LM4H (digital microphone)

### I2C
- IMU motion (accelerometer/gyro/compass) [GY-521](https://a.co/d/03i20tGs), LSM6DS3
- Time-of-flight distance [VL6180X](https://www.pololu.com/product/2489), [VL53L0X](https://www.pololu.com/product/2490)
- Real-time clock DS3231
- Oled display SSD1306
- I2C backpack on LCD1602 character display

### SPI
- APA102 addressable LED strip [sparkfun tutorial](https://learn.sparkfun.com/tutorials/apa102-addressable-led-hookup-guide/all)
- Shift registers HC595 SIPO, HC165 PISO
- LCD TFT display ili9341
- W25Q Flash storage
- SD card reader <https://a.co/d/07BkfTaH>
- Arducam 2-Megapixel Camera OV2640 (uses both i2c and spi)


## Component suppliers

Here are some options of known good sources.

__Local__
- [MicroCenter](https://www.microcenter.com) recently opened in Santa Clara, neat! Their business is mostly consumer electronics along with some hobbyist tools and supplies. In additional to standard electronic components, they also have an eclectic selection of specialized leds/sensors/modules from the Adafruit and Sparkfun catalogs. It's fun to go and browse. If looking for a specific part, check the website inventory before you make the trip.

- [Jameco](https://www.jameco.com/) stocks a large catalog of electronic components and are located in Belmont near SFO.
If you place an online order, parts will generally arrive the next day via US mail. If you have a car, you can also drive there and pick up parts same day at the Will Call desk.

- If you have yet to visit [lab64](https://ee.stanford.edu/news/lab64), go now and check it out! The EE department's Maker Space is located on first floor of Packard building. The staff is super helpful and can assist you with using the available tools (oscilloscope, soldering, 3-D printing, laser cutting). They also have a store stocked with electronic components (transistors, op-amps, diodes, etc) and some common sensors and motors.

__Online__

If ordering online, try to do it early so you aren't having to pay through the nose for expedited shipping.
- [Adafruit](https://www.adafruit.com/) Limor Fried has founded a company that caters to makers of all types. Besides building great products, they also provide extensive tutorials and learning guides on their web site.

- [Sparkfun](https://www.sparkfun.com/) is another great source for hobbyist electronics, many great tutorials.

- [Pololu](https://www.pololu.com/) specializes in parts for robotics (sensors, motor drivers, mechanical). They also document their parts very well.

__Loaners and scrap parts__
- We have a small lending library of some components that we can loan for you to try out before ordering your own. We have the parts demoed in lecture (switches, hall effect, phototransistor, IR, ultrasound, shift register, buzzer, motors...) and other oddballs (RTC, temp, displays). Ask us if you have need for something and we'll try to round up what we can.
- The drawer labeled "project parts" in lab room has a random smattering of scrap parts left over from previous projects. You are welcome to paw through and re-use what you can.
- We also try to keep some common "consumables" on hand: protoboards, heat shrink, ribbon cables, hot glue, connectors, etc. Check the drawer and let us know if supplies are running low so we can replenish.
