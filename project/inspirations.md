---
permalink: /project/inspirations
title: 'Project musings and inspirations'
toc: true
---

The direction you take the project is open-ended. The best project is one that that you thought of yourself and that resonates with your interests.

It is fun to see what others have done and can help get your creative juices flowing. Check out our [project gallery](/project_gallery/) for highlights of past quarters -- what a diverse array of clever projects we have seen!

Some prolific maker sites share field reports about the cool things they have concocted:
+ [Hackaday](http://hackaday.com)
+ [Adafruit blog](http://adafruit.com/blog)
+ [Instructables](http://instructables.com)
+ [Hackster](http://hackster.io)
+ Everything done by [Mark Rober](https://www.youtube.com/MarkRober) does is crazy-awesome
+ [StuffMadeHere](https://www.youtube.com/c/StuffMadeHere) makes impressive systems that blend mechanical engineering, electrical engineering, and computer science.

### Inputs
One fun way to extend your Mango Pi is adding support for another input device. A few ideas of classic input devices you might consider:

- Homebrew game pad constructed from buttons and switches
- Rotary dials/wheels, joysticks
- PS/2 mouse, trackpad
- Re-purposed old-school controller (Atari 2600 paddle, NES/SNES gamepads, Wii nunchuk, etc.)
- Scanner, code reader, magnetic card swiper

You can also create a wholly novel way of controlling your Pi via sensors that detect changes in light, pressure, temperature, motion, distance, sound, etc!

### Outputs
You've taught your Pi to turn on LEDs, control a 7-segment display, send formatted text over uart, and draw to the framebuffer. What other ways might your Pi communicate?

- Light: RGB leds, IR transmitter
- Sound/vibration: generate sound/music, give haptic feedback
- Motion: solenoids, motors


### Games
Implementing a game has been a perennial favorite for projects. We have seen classic games re-implemented with a novel twist (e.g. Tilt-tris, Clappy Bird, BattleDarts) -- neat! Even better is a game of your own design that offers entertaining gameplay through a unique combination of input and output.

### Graphics
The graphics library you built for libmymango is a good start but the needs of your project might inspire you to extend it to add fancier features such as:

- Richer drawing primitives (fill/frame, ovals, anti-aliasing, additional fonts, wireframes)
- Support for frame composition, layering, sprites, blending
- Boost performance to keep up with a higher frame-rate animation
- Rework your fb module to use spi/i2c communication with small LCD/OLED display

The D1 has a very capable Display Engine peripheral with fancy features for 2-D graphics processing: image compositing, multiple layer overlays, scaling, sprite animation, video mixing, and more. There is lots of nifty functionality, but documentation on how to get use is rather sparse, so will require some bushwhacking to blaze trails.

### Sound/music

Many past projects have incorporated music and sound, either as an embellishment or primary project component. PWM can be used to generate simple tones by frequency; I2S gives you a full range of audio processing both input and output.

- Combine input device and audio output into new musical instrument
- Music plus graphics = sound visualization
- Add midi in/out support and interface with midi devices
- Software that allows user to modify/mix sounds (dj board, sound production, autotune)

### Operating systems, networking
Thinking about CS112/CS140E and want to get a jump on it? An ambitious option would be to inmplement one of these operating system components:
- memory system (memory protection, caching, virtual memory)
- filesystem (read/write from sd card)
- multi-threading (allow multiple threads of execution)

What about adding a networking capability that allows multiple Pis to communicate? You could connect TX and RX pins to a shared line or communicate using sound, light, or radio waves.

Wifi and Bluetooth are support by the RealTek RTL8723ds chip on the Mango Pi. Recent Linux distributions have support for both, but we have yet to get either working in our bare metal world, perhaps this will be your contribution!



