---
title: How to make hardware go brrrr
toc: true
attribution: Written by Ashley Ragiosa
---

You're in the home stretch! And I'm sure now you're wondering how you can unleash your wonderful embedded systems skills on new projects. As I've worked on projects over the last years I've come across many odd nuances with hardware and I hope this guide can provide a bit of clarity on selecting the hardware that can "do the job."

I am writing these thoughts down not as an end all be all for the only devices you can pick. I am more so trying to draw from experiences I had (Ashley here) using them and explain some common things to look out for. My best recommendation would be to scope out what you need that piece of hardware to do and read the datasheet and validate it can do said thing. If you find something interesting that would be good to add to this "cheat sheet" let me know.

## Must checks for any piece of hardware

*What is the required power?*

Ensure that you have an appropriate power supply (voltage/current) to drive your chosen devices. To throw out an example, some devices only take 12V input (such as some motor drivers), others might require 5V (such as some SD card breakouts). Calculate that you have enough wattage to supply all your devices with the juice they need.

*Can it be driven by my board?*

Some things to consider:
- VIN or V+ as a label tends to refer to the input voltage it requires. This can be a range of values (e.g. 3.3V - 5V). This means anything in this range is acceptable
- Logic level: this is not the same as power input. This is what the GPIOs need to drive at to do logic for the device. You want to get something that is 3.3V logic because the Mango Pi's GPIOs are 3.3V

*Does it have a (good) datasheet?*

My biggest mistake has been ordering components without looking at the datasheet first.  What I mean by this is some cheaper components may not include a datasheet at all or just a single page of facts and figures without any explanation. While some components may not require a datasheet, not having one could be a sign that you are buying a cheap version of a device that may have other lurking issues. To provide an example, I bought a SD card breakout, realized there was no datasheet, and proceeded to discover there was next to nothing to be found on on the internet about this device and about the signal integrity issues I was having. Sometimes the most value is in knowing other people have tried things out on this device (and that it actually works).

*Have I connected all my grounds together?*

> You should tie all your grounds together on your devices. This is an incredibly common mistake. Let's say I'm trying to talk over UART. If the grounds aren't tied, you saw in lab at the beginning of the quarter it'll be just garble-gook. This also applies to motors (tie the grounds together) and other devices. The reason why this occurs is your reference for your voltage must be consistent.
{: .callout-danger-invert}

## SD cards and SD card readers

If purchasing a SD card or card reader, this is just not an area to skimp out on. __Sometimes dirt cheap SD components are dirt cheap for a reason.__

__microSD cards__
- To provide a little anecdote, I've bought some pretty cheap micro-sd cards before and paid a steep price in time and frustration dealing with their flaky, corrupt nature. Go for a name brand of good quality from a reputable seller, such as SanDisk or Samsung (and make sure they're not counterfeits).
- Go with a 32GB or smaller card and install FAT32 filesystem. This has always provided the least resistance. There's some weirdness here I've never really been able to put my finger on with slower performance as we fill the card and sometimes even just straight up incompatibility due to the card format (such as exFAT). Although, I'm sure there's workarounds to get a >32GB card working just fine.

__SD card reader breakouts__
- I've seen some pretty awesome projects that read/write data from a micro-sd. Once upon a time, I decided to cheap out on a breakout board, picking the WWZMDiB because I could get each for ~$1. Sounded like a steal. Here's what it's missing that a better quality reader will have:
    - No chip detection (this can make it harder to troubleshoot if your SD card is being picked up properly)
    - The signal integrity was the absolute worst. This means I'd get reads that were intermittently corrupted and writes that sometimes failed. Sometimes it becomes apparent that trace layout/quality, poor power delivery, knockoff ICs, and so forth may be playing a role rather than your software
- I found breakouts like the Adafruit brand to be more reliable (and better documented). The ADA254 MicroSD Card Breakout Board was a winner.

For non-volatile storage needed (such as retaining game high scores over reboot), a breakout board with SPI flash such as W25Q128 could be just the thing.

## Motors

When I did my CS107E project I worked on driving steppers to move a marble in a box of sand to draw fractals. We dubbed it a zen sand plotter, although my biggest irk was that it wasn't all that "zen" due to the annoyingly loud stepper motor. Ther are some interesting features built into motor drivers to consider as a consequence! Let's break down a couple of things to think about

Various types of hobby motors that may be of use in a project:
- servos:  PWM signal, no driver required. Good at holding position, limited rotation, slow speed (*E.g: simple lever that needs to open/close*)
- stepper motors: move in discrete steps and pulse a driver to make a step occur (*E.g.: CNC machines, plotters*). This will satisfy most projects CS107E students do
- dc motors: PWM speed control, good for wheels, fans, continuous rotation
- and many more motors which have much higher torque which likely aren't necessary for most CS107E projects

I found it safe to pick motors that I saw already in use in other devices. To give an example, the Prusa 3D printers use Nema17, so that's why I used in my project.

You will need a motor driver board for a stepper motor. Here are two options that may be of interest that I've found useful:
- A4988T: this can provide higher torque. Although if you drive a stepper with it, the motor will sound loud!
- TMC2226: lower overall torque (although fits most applications. I've used this, for example, to pull a device on a fishing line). It will be almost inaudible when driven slowly. This is pretty awesome. It has a mode built in called "Stealth Chop" where you can talk to it over UART to configure the perfect settings to make the motor drive quietly. Although, even out of the box without this configuration, it can drive a motor pretty quietly.

In this video, the A4988T is the first (loud!) and the second is the TMC2226. Previously, I've seen people use these louder motor drivers to actually tune the frequency and make the steppers vibrate at the right frequencies to play songs. It truly depends on what you want:

<iframe width="315" height="560" src="https://www.youtube-nocookie.com/embed/mUId9J_9dEI" title="A4988T vs TMC2226 stepper driver comparison" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

> Do note that an active motor driver chip can get quite hot. I would recommend installing the heatsink (that came with the board) for thermal relief.
{: .callout-warning}

## Sensors

Some chips and breakout boards package multiple types of sensing into one device which is super convenient. An example is the BME680 which does temperature, humidity, barometric pressure, and gas (VOC) detection. Some things to consider with these types of sensors:
- Many have burn-in periods. You commonly need to run them for 24-48 hours continuously before readings stabilize. I've ended up on some silly forums where blowing on it for a period of time "fixes it" although I found I just looked a bit silly. With these types of devices you can use the IAQ Accuracy on the device (it will output this value) to determine if it's calibrated

## LEDs

Addressable strips of RGB LEDs are pretty sweet. Transmitting data on a single GPIO can control the entire strip of LEDs. These are strips of lights that can be cut up and soldered together to make all sorts of cool stuff (think you can solder them to be a cube shape or a grid to do fun displays). Some to consider are __WS2812B ("neopixel")__ and  __SK9822 ("dotstar")__.

- I've personally only tinkered with the WS2812B and they work pretty sweetly. Although they're incredibly timing sensitive (you need to drive for the correct periods of time to get the lights to show properly). Alternatively, the SK9822 uses standard SPI which may feel more out of the box given you have the library from the teaching team.
- The WS2812B is much slower than the SK9822. It's not insanely slow but if you want to do some complex stuff and avoid visible flicker the SK9822 might be your pick. Reading from the datasheet, the WS2812B operates at ~400Hz and the SK9822 at ~1.2 kHz

## IMUs

There's a lot to consider here. Definitely read the datasheet first for any IMU you'll buy. Consider:
- resolution/dynamic range of measurements (what precision do you get)
- sampling rate (Hz), how quickly do you get measurements
- 6-axis vs. 9-axis
    - 6-axis: tilt + angular rate
    - 9-axis: adds electronic compass

The ubiquitous MPU6050 is a 6-axis IMU that can act as an accelerometer/gyroscope and is pretty sane from my experience. For a 9-axis, consider an ICM-20948.

> Quick note: run the device self-test and basic calibration (gyro bias, accel scale, magnetometer calibration, and so forth). It is more work but a great sanity check for your driver and skipping it often hides bugs. Check the device datasheet for what this looks like for your device.
{: .callout-info}

## Wireless communication

Historically, things like bluetooth and WiFi are pretty difficult to get right. Adopting an industrial-strength protocol means taking on a huge set of features such as encryption, error correction, and lots of bells and whistles that you don't necessarily need. If you want two devices talking to one another, there are  simpler options.

A device I've used before was the nRF24L01 which is a pretty fun/cheap way to send packets over radio that can be either acked/non-acked. Their range is not amazing but can get you off the ground fairly quick to communicate at a mild distance.

## And much more to come...
I hope this is more like a living document and we can continue to add advice. But I hope the above gets your thoughts flowing!