COMPARATOR
----------
Show how GPIO can act as comparator.

Demo

1. Potentiometer acting as voltage divider (3.3V/gnd from Pi), connect COMPARE gpio to wiper output
2. Voltage above 1.67V reads as 1, below is 0.



ANALOG TO DIGITAL CONVERSION
----------------------------
Basic idea is to time how long it takes to charge a capacitor.

This technique was used in the original Pong game.
http://atariage.com/forums/blog/52/entry-392-paddles/
Woz also used this approach in the Apple 1.

Demo

1. A 2N7000 mosfet is used to discharge the capacitor, control by DISCHARGE gpio.
   Write 1 to DISCHARGE gpio turns on the mosfet and discharges the capacitor.

2. Connect 10K pot to Pi 3.3V, wiper through 1K resistor to 1uF capacitor.
SIGNAL gpio connects to wiper-resistor node.

3. Write 1 to DISCHARGE, wait for capacitor to charge (read SIGNAL until high), measure elapsed time.
