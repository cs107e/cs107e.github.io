---
title: "Components for Assign 2 extension"
---

The clock assignment extension has been recently redesigned. We hope this new version gives you a fun way to build on the work you put into your gpio/timer modules and perhaps
sprout a few inspirations that resurface in final projects.

We have a smattering of simple input and output devices available for you to choose from. If you have access to different devices of your own that you want to try, come chat with us to see how it can work out.

Here are some notes and resources for the input and output devices.  The manufacturer's datasheet sometimes leaves much to be desired, so we suggest some tutorials that can be a helpful supplement.


### Inputs
- __Rotary encoder knob__
    - Can turn clockwise or counterclockwise. Some versions have pushbutton on knob, can aslos turn while pressing. Very satisfying clicky mechanical feel. I also have a handful of a version without center press where shaft has LED that turns on/off.
    - Possibilities: turn slow one-click-at-time to fine tune, quick sweep to make large change? use press in conjunction with turn to switch between small vs large steps?
    - You can seat it into breadboard with some prodding, but prone to popping out due to short, spindly legs
        - Try soldering header pins to the legs (see photo below) for sturdy placement into breadboard. Good soldering practice, too! Soldering supplies are available on bench in back corner of our lab room. Ask one of our staff of the pros at lab64 for a demo if you are new to soldering.
        ![rotary with soldered header](../images/rotary_with_header.jpg){: .w-25 .zoom}
    - The manufacturer [datasheet](../datasheets/EC11.pdf) is pretty primitive, lighted shaft [datasheet](../datasheets/LightedPEL12S.pdf) is easier to read. An [overview of how a rotary encoder works](https://learn.adafruit.com/rotary-encoder/overview) from Adafruit.
- __5-way navigation switch__
    - Like a mini-joystick, rock to compass points (NSEW), push down on center. Buttons are exclusive (can only activate one at a time).
    - Possibilities: press and release once to fine-tune, press and hold to make large change? move between minutes/seconds and set separately?
    - Comes mounted on a breakout board, header unsoldered. Solder on header and you'll have easy connect via jumpers.
         ![5-way unsoldered](../images/5way_unsoldered.jpg){: .w-25 .zoom}
    - Manufacturer [datasheet](../datasheets/SKQUCAA010-ALPS.pdf) for switch. It is basically 5 push buttons bundled into one component. Pinout labels are on underside of board.
- __Rotary navigation wheel__
    - Mash-up of a rotary encoder and a 5-way switch -- reminiscent of old iPod for those retro technology enthusiasts.
        ![5-way unsoldered](../images/rotary_wheel.jpg){: .w-25 .zoom}
    - Possibilities: everything a rotary encoder can do plus a 5-way navigation switch. Overkill?! :-)
    - Comes unmounted, solder to [breakout board from Adafruit](https://www.adafruit.com/product/5221) for ease of connection.
    - I have just a few of these.
    - Manufacturer [diagram](../datasheets/ANO+Encoder.jpg)
- __Membrane keypad__
    - Reports press of single button and multiple concurrent buttons.
    - Possibilities: uh, it's a number pad... clear affordance, fewer options for creativity, a little less interesting. Error-check user input (too large, out of bounds)?
    - Has dupont connector, run jumpers to breadboard.
    - The datasheet for this particular keypad was nowhere to be found, but here is a [summary](https://protosupplies.com/product/membrane-keypad-4x3-matrix/) provided by the supplier (ProtoSupplies).

![input devices](../images/ext_input.jpeg){: .zoom}

### Outputs

- __Passive buzzer__
    - Tiny little speaker, sound quality not bad, retro feel
    - Possibilities: play notes/melody, make sound effects (sci fi laser gun, sad trombone), fun!
    - Seats like a champ on a breadboard
    - [Specs](https://www.amazon.com/dp/B07GBTJP9F) from supplier, an [overview of passive buzzer](https://protosupplies.com/product/passive-buzzer-5v-module/) from ProtoSupplies
- __Vibrating disc motor__
    - Motor is plucky for its size. Try attaching to a coin and sit on a desk to further amplify the motion.
    - Possibilities: buzzy patterns, ramp up/down, S-O-S ?
    - Tiny gauge wires not great for direct connect to breadboard (wiggles out when vibrating). Best to solder a graft onto larger gauge hookup wire/jumper for robust connection.
    - Manufacturer [datasheet](../datasheets/P1012.pdf), general [info](https://www.adafruit.com/product/1201) from vendor
- __Common cathode RGB LED__
    - Basically three LEDs inside a diffused casing (red, blue, green)
    - Possibilities: ramp dim->bright, color cycle
    - Perfect fit on a breadboard
    - Tweaking resistor value per individual RGB may help with achieving a more balanced white
    - Manufacturer [datasheet](../datasheets/YSL-R1047CR4G3BW-F8.pdf), an [overview of RGB led](https://protosupplies.com/product/led-rgb-5mm-clear-cc/) from ProtoSupplies

![output devices](../images/ext_output.jpeg){: .zoom}

Come find me (Julie) to pick up a device. We ask that you return the input device to us so we can reuse them, but you are welcome to keep the output devices.

If you have a different device of your own that you want to use instead, let's talk!
