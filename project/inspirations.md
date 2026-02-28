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
One fun way to extend your Mango Pi is adding support for another input device. You could write a driver for a classic old-school input device such as:

- Homebrew game pad constructed from buttons and switches
- Rotary dial, wheel, joystick
- PS/2 mouse, trackpad
- Re-purposed retro controller (Atari 2600 paddle, NES/SNES gamepads, Wii nunchuk, etc.)
- Scanner, code reader, magnetic card swiper

You can also create a wholly novel way of controlling your Pi via sensors that detect changes in light, pressure, temperature, motion, distance, sound, or others.

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

Many past projects have incorporated music and sound, either as an embellishment or primary project component. PWM can be used to generate simple tones by frequency; I2S gives you a full digital audio both input and output.

- Combine input device and audio output into new musical instrument
- Music + graphics = sound visualization
- Add midi in/out support and interface with midi devices
- Software that allows user to modify/mix sounds (dj board, sound production, autotune)

### Operating systems, networking
Looking forward to CS112/CS140E and want to get a jump on it? An ambitious option would be to inmplement one of these operating system components:
- memory system (memory protection, caching, virtual memory)
- filesystem (read/write from sd card)
- multi-threading (allow multiple threads of execution)
- networking to allow two Pis to communicate
    - connect TX and RX pins to a shared line or wirelessly ommunicate via sound, light, radio waves
    - design your own networking protocol!

### On-board hardware peripherals
There is a lot of functionality packed within the Mango Pi board that you may want to explore. The simpler hardware peripherals (such as the IR transmitter/receiver, thermal sensor, RTC real-time clock, LEDC neopixel control, crypto engine) would be quite feasible targets for writing your own driver. However, the industrial-strength protocols (e.g. USB, SHMC, HDMI, MIPI DSI, Ethernet, DMIC, Audio/video codecs) are a major challenge to bring up starting from bare-metal, not that suitable for final project.

#### D1-H peripherals
The hardware peripherals listed below are part of the SOC. Refer to [D1-H User Manual](/readings/d1-h_user_manual_v1.0.pdf) for more info. We have support code available for some of these, but many are yet to be explored.

+ CCU (clock controller) Ch 3.2
    - many peripherals require clock config as part of init
    - source for reference module `$CS107E/src/ccu.c` ([browse](/src#ccu))
+ DMA (direct memory access) Ch 3.9
    - DMA used for fast data transfer to/from peripherals, offload from CPU
    - robust and full-featured DMA driver completed by Curis team
+ THS (thermal sensor) Ch 3.10
    - monitor for temperature of chip
+ RTC (real-time clock) Ch 3.14
    - Mango Pi does not maintain independent power for RTC domain
    - if backup power needed, consider instead battery-backed external RTC, e.g. DS3231
+ Graphics Ch 4
    - Display Engine, G2D (hardware acceleration 2-D graphics), Video Engine
    - source for reference module `$CS107E/src/de.c` ([browse](/src#de))
    - Spec for [Display Engine 2.0](/readings/Allwinner_DE2.0_Spec_V1.0.pdf)
+ Video output Ch 5
    - pipeline of pixels to display hardware (HDMI, LCD, MIPI, TCON)
    - source for reference module in `$CS107E/src/hdmi.c` ([browse](/src#hdmi))
+ SMHC (read/write sd card) Ch 7.2
    - draft version of boot disk, reads boot program from sd card FAT filesystem
    - <https://github.com/cs107e/sdcard_boot_spl>
+ Audio (I2S/PCM, DMIC, OWA) Ch 8
    - draft I2S driver from Curis team
+ TWI (Two-Wire,i.e. i2c)  Ch 9.1
    - robust and full-featured I2C driver completed by Curis team
+ UART (async receiver/transmitter) Ch 9.2
    - source for reference module `$CS107E/src/uart.c` ([browse](/src#uart))
+ SPI (serial peripheral) Ch 9.3
    - robust and full-featured SPI driver completed by Curis team
+ USB (universal serial bus) Ch 9.6
    - would be big job...
+ ADC (analog-digital conversion) Ch 9.8-9.10
    - several ADCs avail on-chip, but no analog inputs  broken out on Mango Pi board, sigh
    - consider instead external ADC, e.g. MCP3008
+ PWM (pulse width modulation) Ch 9.11
    - draft code, needs further testing
+ LEDC (led controller) Ch 9.12
    - supports precise timing for neopixel-like rgb led strips, offload from CPU to hw peripheral
+ EMAC (ethernet) Ch 9.13
    - would be big job...
+ IR (infrared receiver/transmitter) Ch 9.14-9.15
    - hw peripheral for encoding/decoding protocol, use in conjuction with IR detector and/or emitter
+ Security crypto engine Ch 10

#### Wifi/bluetooth
There is a RealTek rtl8723ds wifi and bluetooth module on-board the Mango Pi. Sadly, documentation is hard to find and unelpful (Realtek proprietary/secret?).
One of our original Mango Pi-oneers was very eager to get BT working. It proved much too ambitious for CS107e, so he pivoted that portion of his project to use an ESP-32. But he followed up his BT dreams in 140E/240LX to create a wireless bootloader, wow!  We can put you in touch with Javier if you would live to learn from or build on his code.

#### Vector unit
The RISC-V processor of the MangoPI includes a vector unit that can be used for SIMD operations. Former student Yifang Yang wrote up a [starter guide](vector_unit).




