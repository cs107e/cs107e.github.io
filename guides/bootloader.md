---
title: Using the bootloader
---

*Written by Pat Hanrahan*

During development, you will edit and compile programs on your laptop,
but when you are ready to execute the program you need to transfer it to the Pi.
You could do this using a micro-SD card where after each re-compile, you
copy the freshly compiled program from your laptop onto the card, eject the
card and insert into Pi, but that process quickly becomes tedious!

Instead, we will use a __bootloader__. The bootloader is a program
that runs on the Pi and listens on the serial port for commands and data
coming from a connected computer. On your laptop, you run a script
to send your compiled program over the serial port to the waiting 
bootloader. The bootloader receives the program and writes it to the
memory of the Pi, a process called "loading" the program. After the
program is loaded, the bootloader jumps to the start address of the program,
and the program begins to run. To stop that program and start another,
you will reset the Pi and use the bootloader again.

Instead of using an SD card to move programs from your laptop to the Pi, 
using a bootloader, the cycle to run a new program is just:

1.  Reset the Raspberry Pi (by unplugging the Pi from your computer or by
    pushing the reset button if you have soldered one on)
2.  Use `rpi-install.py` to send your program to the bootloader

The bootloader we are using was developed by David Welch and modified by Dawson
Engler and Julie Zelenski. It is a modification of David Welch's bootloader06. 
If you have some time, we highly
recommend you explore his [github repository](https://github.com/dwelch67/raspberrypi).

This document will walk you through the steps to use the bootloader.

### Configure Pi's micro-SD card to run the bootloader

You should have previously set up your micro-SD card with the 
[firmware files](https://github.com/cs107e/cs107e.github.io/tree/master/firmware) 
as directed in the [SDHC card guide](/guides/sd).

The firmware files include the program `bootloader.bin`. Insert the SD card into your
laptop and copy `bootloader.bin` to `kernel.img`. Eject the card and install 
into the micro-SD slot on your Pi. The next (and every subsequent) time that you reset the
Pi with that micro-SD card installed, the bootloader will run.

### Connect your laptop to the Pi

In this course, we will be using a CP2102 USB serial breakout board. The CP2102
is the chip that converts from a USB interface to a serial interface. On one
end of the breakout board is a USB Type A connector. The breakout board plugs
into any USB port on your laptop. The other end contains a
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

Use female-female jumpers to connect the TX and RX pins on your Pi to the TX and RX pins on the USB breakout
board. The proper connections are shown below. Note that your USB breakout board
may have pins in different positions. Don't just follow the picture blindly!

![Console cable](../images/console.cable.zoom.jpg)

In this configuration, the green wire connects
the RX header pin on the USB serial breakout board
to the TX GPIO on the Pi's header.
The blue wire connects the TX header pin
to the Pi's RX GPIO.

### Installing the serial port driver

If you are on a Mac, ensure you have followed the instruction for the console
drivers in the [Mac toolchain guide](/guides/mac_toolchain). On Windows or
Linux, you don't need to do anything special here.

### Load and run your program

Our provided `rpi-install.py` program is used to send a binary
file to the bootloader.  

To load and run `blink-actled.bin`, simply type:

    % rpi-install.py blink-actled.bin 
	Found serial port: /dev/cu.SLAB_USBtoUART
	Sending `blink-actled.bin` (72 bytes): .
	Successfully sent!
    
After a brief pause, you should see the green LED on the Pi slowly blinking.

If you change your program and wish to reload it onto the Pi, 
you must power cycle the Pi (press the reset button if you have installed 
one or simply unplug the USB-serial). The Pi will reboot into 
the bootloader and once again wait for a new program to be loaded.


### Troubleshooting 
<a name="troubleshooting"></a>

If you are having trouble using the bootloader to send programs
to the Pi, work through this checklist to find and resolve
your issue.

1. Reset Pi and verify bootloader is running by looking for its "heartbeat".

	- When the bootloader is running, it repeatedly gives 
	two short flashes of the ACT LED (the green LED on the Pi board).
	This "da-dum" is the heartbeat that tells you the 
	bootloader is ready and listening.

	- If your Pi doesn't have a heartbeat, the most common cause is because
	you have already bootloaded a program and need to reset the Pi to
	restart the bootloader.	Do that now!  
	
2. If bootloader does not run after you reset Pi, check the micro-SD card.

	- Be sure that the micro-SD card is fully inserted into the slot on the underside of the Pi.

	- Verify the contents of the micro-SD card by inserting into your
	laptop. It should have a file named `kernel.img` that is a copy
	of the file originally called `bootloader.bin` from the
	 [firmware](https://github.com/cs107e/cs107e.github.io/tree/master/firmware) directory.

3. If bootloader is running on Pi, but your laptop fails to communicate with it, check your connections.

	- The TX and RX from your USB-serial breakout board should be connected to
	 RX and TX on your Pi.  Remember: TX should go RX and 
	 vice versa (review the diagram above).





