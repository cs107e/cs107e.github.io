==== Pat ====

Normally MIDI uses 5V, but since it is a current loop
it can also be run at 3.3V. 

MIDI spec says 5 mA of current in the current loop.

doc/pi.midi.jpq shows a picture of a MIDI breakout board
with MIDI IN and OUT DIN connectors. The schematic is
doc/pi.midi.schematic.jpg.

MIDI OUT is connected to GPIO26 using the green wire.
Green is our color convention for PI TX and HOST RX.

MIDI IN is not used in these examples.
It would be connected to PI RX and HOST TX.
MIDI IN is routed through the 6N138 optoisolator.

The current circuit uses 220 Ohm resistors as is typical
of the MIDI spec. It would be safer to lower the value
of the resistor by 3.3V/5.0V to keep the current
constant with 3.3V. 

An inexpensive USB IN-OUT MIDI cable can be used to
connect to a laptop. I connected this to my macbook,
and was able to play music using GarageBand. I also
used MidiMonitor to show the midi notes as they were
being sent.

Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
Date: Feb 18, 2019, Updated Mar 9, 2021

===== Phil ====

Sample code to control a KORG volca keys or volca beats
from the Pi. Connect the midi data line to GPIO pin 25
(through a 220Ohm resistor) and the current line to
5V through a 220Ohm resistor.

1. Makefile:
  make on its own will make midi-beat (drums on the drum kit)
  make install will make and install midi-beat
  
  make install midi-lamb will make and install midi-lamb
  
2. MIDI-in pins:
  white wire to 220ohm resistor to 5V on Pi
  red wire to 220ohm resistor to pin BCM/GPIO 25

3. For PulseView logic analyzer with DreamSourceLab DSLogic Pro:
  a. Ground goes to Pi ground
  b. Signal pin goes to red MIDI (pin 25 on Pi)
  c. Set voltage range on PulseView to 1.6V for 0/1
  d. Set UART as decoder, with 31250 baud rate (8n1) (for Mango Pi: baud needs to be roughly 33500)
  e. In UART, choose "Stack Decoder" and select MIDI
  
4. Actual pinout of MIDI connector: (updated, May 2024)

- original cable pinout was red=signal, white=5V
- new cable pinout: 

            silver (unshielded), gnd when using logic analyzer
                    _____
                 .-'     '-.
       yellow  .'     o     '.  red
      (signal)/   o       o   \ (5V)
             ;                 ;
     white   |  o           o  |   black
   (no conn) ;                 ; (no conn)
              \               /
               '.           .'
                 '-._____.-'



Author: Philip Levis <pal@cs.stanford.edu>
Updated for new libraries by Chris Gregg <cgregg@stanford.edu>
Date: May 14, 2017
