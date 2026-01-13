---
title: "Guide: Powering the Mango Pi"
attribution: Written by Julie Zelenski
---

We will supply power to the Mango Pi via usb. Find the two USB-C ports along the edge of the Mango Pi board. The port that is closer to the corner is labeled `OTG`.
USB On-The-Go (OTG) is a USB sub-specification  that allows devices to switch between the roles of host and device. Connecting a 5V power source to either usb port will power up the Pi, but we connect to the OTG port because it additionally suppports the communication protocol used by the [xfel bootloader](/guides/xfel) we will be using to load and execute programs on the Pi.

Connect a usb cable from a usb port on your laptop to the OTG port on the Mango Pi. The green power LED turns on when the Pi is receiving power. It is blindingly bright! We wish it were possible to dim it. If you find a way, please share.

![usb otg power](../images/power-otg.jpg)
{: .w-50 .mx-auto }

The Pi does not need much power. We can measure how much power it requires by using an inline power meter. The usb specification says that a usb port should supply 5V, and up to 500 mA of current.
Our experiment shows that when the Pi is plugged it, the usb port has a voltage of 5.12V and is supplying 80 mA of current.
The total power being consumed is 0.41 W. That is not a lot of power! The power used may go up if you are using LEDs and other peripherals.

![usb power meter](../images/power-meter.png)
{: .w-75 .mx-auto }

To reset the Pi, simply interrupt its power supply. One way to do this is by briefly unplugging and re-plugging the usb cable. We provide a usb cable that has a built-in switch to make the process a little easier.

The latest (Dec 2025) rev of the Mango Pi board __includes a reset button__! 🔥  Look for the tiny push button on the edge of the board between hdmi and USB connectors. Click this button to reset the Pi.
