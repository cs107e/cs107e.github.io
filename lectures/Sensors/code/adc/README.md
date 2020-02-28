Timing based Analog to Digital Convertor.

The basic idea is to time how long it takes to charge a capacitor.

This technique was used in the original Pong game.

http://atariage.com/forums/blog/52/entry-392-paddles/

Woz also used this approach in the Apple 1.


Demo

1.A 2N7000 mosfet to discharge the capacitor. Setting GPIO_3 to 1 
turns on the mosfet and discharges the capacitor.

2. A 10K pot was connected to 3.3V from the Pi. The wiper was connected
to a 1K resistor to a 1uF capacitor.
GPIO_2 was connected to the wiper-resistor node.
The GPIO pin is a comparitor, 
When the valtage goes above 1.67V, it reads as 1.

