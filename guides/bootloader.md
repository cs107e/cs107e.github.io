---
layout: page
title: Using the console cable and bootloader
---

*Written by Pat Hanrahan*

In the [SDHC card guide](/guides/sd), you should have installed a bootloader on
the SDHC card as `kernel.img`. When the Raspberry Pi runs, the bootloader is
run. The bootloader listens on the serial port for commands to load a program
into memory on the Pi. After the program is loaded, the bootloader jumps to the
start address of the program, and the program begins to run. If you want to run
a new version of your program, you reboot the processor, and download a new
version of your program.

So each time you wish to run a new version of your program, you only need to do
two steps:

1.  Reset the Raspberry Pi (by unplugging the Pi from your computer or by
    pushing the reset button if you have soldered one on)
2.  Run the command to load and start your program

In order to use the bootloader, you need to format an SDHC card with the
bootloader installed as `kernel.img`. You also need to connect your console
cable to your laptop and the Raspberry Pi. Finally, you need to run a script to
send the program to the Pi.

### Install the bootloader on the Pi

The bootloader we are using was developed by David Welch and modified by Dawson
Engler. It is a modification of David Welch's bootloader06. 
If you have some time, we highly
recommend you explore his [github repository](https://github.com/dwelch67/raspberrypi).

We have already compiled a working `bootloader.bin` for you. It is part of the
default set of firmware files, and should already be on your SDHC card. To run
the bootloader, just copy `bootloader.bin` to `kernel.img`. Next time you reset
the Raspberry Pi, the bootloader will run.

### Connect your laptop to the Pi using the console cable

In this course, we will be using a CP2102 USB serial breakout board. The CP2102
is the chip that converts from a serial interface to a USB interface. On one
end of the breakout board is a USB Type A connecter. The breakout board plugs
into any USB port on your laptop. The other end of the console cable contains a
6 pin header. Two of the pins deliver 5V and GND. They can be used to power the
Pi; for details, consult the [Powering the Pi Guide](/guides/power).

Two other pins are used for transmitting (TX) and receiving (RX). The Pi also
has a TX and RX pin on the GPIO header. These pins are on the outside of the
header. Starting at the top, we have

     2 5V
     4 5V
     6 GND
     8 TX 
    10 RX

By convention, the transmit TX on one end is connected to the receive RX on the
other end.

Connect the TX and RX pins on your Pi to the TX and RX pins on the USB breakout
board. The proper connections are shown below. Note that your USB breakout board
may have pins in different positions. Don't just follow the picture blindly!

![Console cable](/images/console.cable.zoom.jpg)

In this configuration, the yellow wire connects
the RX header pin on the usb serial breakout board
to the TX GPIO Pin 8 on the Pi's header.
The orange wire connects the TX header pin
to the RX GPIO Pin 10.

### Installing the CP2102 virtual serial port drivers

If you are on a Mac, ensure you have followed the instruction for the console
drivers in the [Mac toolchain guide](/guides/mac_toolchain). On Windows or
Linux, you don't need to do anything special here.

### Load and run your program

We have created a Python program that sends binary files to the bootloader. If
you have followed all the setup instructions, the program is already installed.
If you are on a Mac and it is not working for you, be sure you followed the
latter part of the setup instructions [here](/guides/mac_toolchain).

To load and run `blink.bin`, simply type:

    % rpi-install.py blink.bin
    Sent True
    %

After a few seconds, you should see the LED blinking.

If you change your program and wish to reload it onto the Pi, 
you must power cycle the Pi (press the reset button if you have installed 
one or simply unplug the USB console cable). The Pi will reboot into 
the bootloader and once again wait for a new program to be loaded.