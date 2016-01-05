---
layout: page
title: Working with the console cable
---

*Written by Pat Hanrahan*

The usb serial cable can be used to communicate between your laptop and the
Raspberry Pi.

One use of the usb serial cable is to implement a console. The console acts as
a terminal connected to the Pi. Another use of the serial cable is to implement
a bootloader. With a bootloader, you don't need to continually eject and insert
the SDHC card. Instead you can send the program you want to run directly to the
Raspberry Pi without first storing it on the SDHC card.

The usb serial cable can also be used to power your Pi.

### Connecting the usb serial cable to the Raspberry Pi 

In this course, we will be using a CP2102 usb serial breakout board. The CP2102
is the chip that converts from a serial interface to a usb interface. On one
end of the breakout board is a usb Type A connecter. The breakout board plugs
into any usb port on your laptop. The other end of the console cable contains a
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

By convention, the transmit TX on one end is connected
to the receive RX on the other end.

The proper connections are shown below

![Console cable](/images/console.cable.zoom.jpg)

In this configuration, the yellow wire connects
the RX header pin on the usb serial breakout board
to the TX GPIO Pin 8 on the Pi's header.
The orange wire connects the TX header pin
to the RX GPIO Pin 10.

### Installing the CP2102 virtual serial port drivers

Another common use of the usb serial breakbout is to
communicate between your laptop and the Raspberry Pi.
When doing this, the usb serial breakout board appears
at a serial device, or COM port.

**Windows and Linux**

There is no need to install any drivers to use the breakout board.

**Mac OSX**

On the Mac, you need to install the 
CP210X usb to uart bridge vcp drivers
Download [Version 3.1](https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx) of the drivers.
Clicking on the download link will download the
file `Mac_OSX_VCP_Driver.zip`.
Opening this file will uncompress and you will see a new file,
SiLabsUSBDriverDisk.dmg.
Opening the dmg file, will mount the volume.
You should see a volume named
"Silicon Labs VCP Driver Install Disk" in the finder under devices.
Going to that folder, you will see an installed named
"Silicon Labs VCP Driver Installer."
Opening that file will launch the installer.
Follow the instructions to complete the installation.
You will need to restart the computer after the installation.

Now plug the usb breakout board into a usb port on your laptop.
Check whether the serial port exists by looking for the device file.

    % ls /dev/tty.SLAB_USBtoUART
    /dev/tty.SLAB_USBtoUART

You will now be able to use the [bootloader](/guides/bootloader) and a terminal
program to communicate with the Raspberry Pi.

