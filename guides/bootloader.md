---
title: Using the bootloader
toc: true
---

*Written by Pat Hanrahan and Julie Zelenski*

### What is a bootloader?
You will edit and compile programs on your laptop,
but you must transfer the compiled program to the Pi to execute it. One way to move a program from your laptop to the Pi is using a micro-SD card.  You would insert the card on your laptop, 
copy the newly compiled program onto it, eject the
card and then insert into Pi. You can see how following up each re-compile with this process would quickly become tedious!

Instead, you can set up a serial connection between your laptop and the Pi and use a __bootloader__ to transfer the program. The bootloader runs on your Pi
and listens on the serial connection. On your laptop, you send the newly compiled program over the serial connection to the waiting 
bootloader. The bootloader receives the program and writes it to the
memory of the Pi, a process called "loading" the program. After the
program is loaded, the bootloader jumps to the start address of the program,
and the program begins to run.

Using a bootloader, the cycle to run a newly-compiled program is just:

1.  Reset the Raspberry Pi to restart the bootloader
2.  Use `rpi-install.py` from your laptop to send your program to the bootloader

The bootloader we are using was developed by David Welch and modified by Dawson
Engler and Julie Zelenski. 
If you have some time, we highly
recommend you explore David Welch's [github repository](https://github.com/dwelch67/raspberrypi) of code for bare-metal development on Raspberry Pi.

This guide walks you through configuring and using the bootloader.

### Configure micro-SD card for bootloader

You should have previously set up a micro-SD card for your Pi with the 
[firmware files](https://github.com/cs107e/cs107e.github.io/tree/master/firmware) 
as directed in the [SD card guide](/guides/sd).

The firmware files include the program `bootloader.bin`; this is the compiled binary for the bootloader. Insert the SD card into your
laptop and copy `bootloader.bin` to `kernel.img`. Eject the card and insert it
into the micro-SD slot on your Pi. The next and every subsequent time that you reset the
Pi, the bootloader will run.

### Connect your laptop to the Pi

You will use a CP2102 USB to serial breakout board (hereafter referred to as just “USB-serial”) to connect your laptop to the Pi. The CP2102
is the chip that converts from a USB interface to a serial interface.

{% include callout.html type="danger" %}
To use the USB-serial, your laptop needs the CS2012 console driver. You should have installed it when configuring your development environment. Refer to the section "CP2102 console driver" in the [installation guide](/guides/install) for your OS.
</div>


One
end of the USB-serial is a USB Type A connector that plugs
into any USB port on your laptop. The other end contains a
6 pin header. Two of the pins deliver 5V and GND. They can be used to power the
Pi; for details, consult the [Powering the Pi Guide](/guides/power).

Two other pins on the USB-serial are used for transmitting (TX) and receiving (RX). The Pi also
has a TX and RX pin on its GPIO header. Review the [Pi's pinout diagram](../images/pinout.pdf) to find the pins labeled `UART TXD` and `UART RXD`. 

Use female-female jumpers to connect the TX and RX pins on your Pi to the TX and RX pins on the USB-serial.  By convention, the transmit TX on one end is connected to the receive RX on the
other end. 

In the configuration pictured below, the green wire connects
the RX header pin on the USB-serial
to the TX GPIO on the Pi's header.
The blue wire connects the TX header pin
to the Pi's RX GPIO. 

![Console cable](../images/bootloader.cable.jpg)

Note that the pins on your USB-serial
may be in different positions or have different label names. Don't just follow the picture blindly!

### Load and run a program
The `rpi-install.py` script sends a binary file from your laptop to the bootloader. Change to the firmware directory and send the blink program:

    $ cd ~/cs107e_home/cs107e.github.io/firmware
    $ rpi-install.py blink-actled.bin 
	Found serial port: /dev/cu.SLAB_USBtoUART
	Sending `blink-actled.bin` (72 bytes): .
	Successfully sent!
    
After a brief pause, you should see the green activity LED on the Pi slowly blinking.

To load and run a different program, your must first reset the Pi by briefly unplugging the USB-serial. This step is necessary to restart the bootloader so it is ready to receive the program you sent via `rpi-install.py`.

<a name="troubleshooting"></a>
### Troubleshooting 
If you are having bootloader trouble, work through this checklist to find and resolve your issue.

1. Reset Pi and verify bootloader is running by looking for its "heartbeat".

	- When the bootloader is running, it repeatedly gives 
	two short flashes of the ACT LED (the green LED on the Pi board).
	This "da-dum" is the heartbeat that tells you the 
	bootloader is ready and listening.

	- If your Pi doesn't have a heartbeat, the most common cause is because
	you have already bootloaded a program which is now running on the Pi. To stop that program and restart the bootloader, you must reset the Pi.	Do that now!   You can reset the Pi by briefly unplugging the USB-serial or using your [reset switch](../reset-button).
	
2. If bootloader does not run after reset, check the micro-SD card.

	- Be sure that the micro-SD card is fully inserted into the slot on the underside of the Pi.

	- Verify the files of the micro-SD card by inserting it into your
	laptop. There should be a file named `kernel.img` that is a copy
	of the file originally called `bootloader.bin` from the
	 [firmware](https://github.com/cs107e/cs107e.github.io/tree/master/firmware) directory.

3. If bootloader is running, but your laptop fails to communicate with it, check your connections.

	- The TX and RX from your USB-serial should be connected to
	 RX and TX pins on your Pi.  Remember: TX should go to RX and 
	 vice versa.
	- If your jumper cables have become stressed/worn, try replacing them with fresh ones.





