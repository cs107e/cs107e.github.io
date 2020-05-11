Sample code to control a KORG volca keys or volca beats from the Pi.

Connect the midi data line to GPIO pin 25
(through a 220 Ohm resistor) and the current 
line to 5.0V through a 220 Ohm resistor.

Author: Philip Levis <pal@cs.stanford.edu>
Date: May 14, 2017

==== Pat ===

Change to 3.3V connection. Since GPIO generates
3.3V when high, if the current source is connected
to 5V, we will get current flowing through the current loop.

Instead, connect the midi data line to GPIO pin 25
(through a 220 Ohm resistor) and the current 
line to 3.3V through a 220 Ohm resistor.

This works, but might be safer to lower the value
of the resistor by 3.3V/5.0V to keep the current
constant with 3.3V. 

MIDI spec says 5 mA of current in the current loop.

Author: Pat Hanrahan <pal@cs.stanford.edu>
Date: Feb 18, 2019

