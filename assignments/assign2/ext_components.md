---
title: "Components for Assign 2 extension"
toc: true
---

Below is info about components we have on hand to loan. If you have access to another device you want to try, likely good, too, come chat with us to confirm.


### GPIO inputs
Use an input device to control the clock or as an user input for a game or interactive application. Implement debounce for cleanest event response!
- __Mechanical buttons__
    - Buttons of various forms, latching and non-latching (e.g push, toggle, slide, lever)
     ![buttons](../images/buttons.jpg){: .w-50 .zoom}
- __Digital switches__
    - Switches that change response to tilt, shock, magnetic field (reed switch), capacitive touch, photo-interruptor
    ![switches](../images/switches.jpg){: .w-50 .zoom}
- __Rotary encoder knob__
    - Turn clockwise or counterclockwise, satisfying clicky feel. Knob may act as pushbutton, some have lighted shaft.
    - [Datasheet](../datasheets/EC11.pdf) from manufacturer is pretty primitive, lighted shaft [datasheet](../datasheets/LightedPEL12S.pdf) is easier to read. [Overview of rotary encoder](https://learn.adafruit.com/rotary-encoder/overview) from Adafruit.
    ![rotary](../images/rotary.jpg){: .w-25 .zoom}
- __5-way navigation switch, mini joystick__
    - Bundle of 5 individual buttons, rock to compass points (NSEW), push down on center
         ![5-way unsoldered](../images/5way_unsoldered.jpg){: .w-25 .zoom}
    - [Datasheet](../datasheets/SKQUCAA010-ALPS.pdf) from manufacturer
- __Rotary wheel__
    - Mash-up of rotary encoder with 5-way switch -- reminiscent of old iPod for those retro technology enthusiasts.
        ![5-way unsoldered](../images/rotary_wheel.jpg){: .w-25 .zoom}
    - [Diagram](../datasheets/ANO+Encoder.jpg) from manufacturer
- __Membrane keypad__
    - 3 x 4 matrix number keypad, reports press of single button and multiple concurrent buttons.
    - [summary](https://protosupplies.com/product/membrane-keypad-4x3-matrix/) from supplier ProtoSupplies
    ![keypad](../images/keypad.jpg){: .w-25 .zoom}

### GPIO outputs
Turning something on or off not so exciting by itself, but using your timer and gpio modules to pulse in specific patterns/timing creates cool effects!
- __Shift register__
    - Can control 8 outputs from just 3 gpios, handy!
    - SN74HC595N 8-bit SIPO
    - [Datasheet](https://www.digikey.com/en/products/detail/texas-instruments/SN74HC595N/277246) from supplier
- __Common cathode RGB LED__
    - Three LEDs (red/green/blue) inside a diffused casing, selective dimming on individual components for full color spectrum and brightness
    - Ramp dim->bright, cycle through colors
    - Manufacturer [datasheet](../datasheets/YSL-R1047CR4G3BW-F8.pdf), an [overview of RGB led](https://protosupplies.com/product/led-rgb-5mm-clear-cc/) from ProtoSupplies
- __Other display units__
    - bar graph (10 individual LEDs), would work well with shift register
    - 8x8 dot matrix 788AS [Datasheet](https://cdn.velleman.eu/downloads/29/infosheets/vmp502_led8x8_1088as.pdf)
- __Passive buzzer__
    - Tiny little speaker, ok sound quality, retro feel, software pwm at frequency can play square-wave notes/melody, make sound effects (sci fi laser gun, sad trombone)
    - Can drive direct from GPIO with series resistor (will be quiet, add transistor will boost volume)
    - [Specs](https://www.amazon.com/dp/B07GBTJP9F) from supplier, an [overview of passive buzzer](https://protosupplies.com/product/passive-buzzer-5v-module/) from ProtoSupplies
- __Vibrating disc motor__
    - Make haptic feedback through buzzy patterns, ramp up/down. Motor is plucky for its size. Try attaching to a coin and sit on a desk to further amplify the motion.
    - Driving a motor directly from GPIO not recommended (even a tiny one), add series resistor to reduce current or make buffer circuit <https://learningaboutelectronics.com/Articles/Vibration-motor-circuit.php>
    - [Datasheet](../datasheets/P1012.pdf) from manufacturer, [info](https://www.adafruit.com/product/1201) from supplier

![output devices](../images/outputs.jpg){: .zoom}

### Circuits, wiring
- Remember the need for pull-up/down configuration to avoid inputs in floating state. For best convenience, implement internal resistor!
- Highly recommend a review of __"Beginners Guide to Wiring Things to the GPIO"__ <https://forums.raspberrypi.com/viewtopic.php?t=216304>, post on Raspberry Pi forum. We pointed our students to this helpful post back in the day; much of its great advice still generally holds in Mango Pi world, especially appreciate its emphasis on protecting the Pi.
- Some components are breadboard-friendly, others have weak/short legs that aren't sturdy so best bet is to solder on header pins. Soldering supplies and header pins are available on bench in back corner of our lab room. Ask one of our staff for help or go to the pros at lab64 for a demo if you are new to soldering. Example of header pins added to rotary encoder:
  ![rotary with soldered header](../images/rotary_with_header.jpg){: .w-25 .zoom}
