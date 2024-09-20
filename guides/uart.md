---
title: "Guide: Serial communication over uart"
toc: true
attribution: Written by Julie Zelenski
---
## What is uart?

UART is an acronym for __Universal Asynchronous Receiver-Transmitter__, a protocol for sequential serial communication.  Most processors include hardware support for a uart peripheral (the Mango Pi has six of them in fact!). A uart is a serial communication channel for sending and receiving data between devices. Your laptop can use a uart to talk to your Pi and receive output and provide input. Adding the capability to your Pi to exchange data with your laptop will be a great accomplishment, akin to the wonderment a new parent feels when their toddler's babbling turns into actual sentences!

## CP2012 USB to serial breakout board

You will use a CP2102 USB to serial breakout board (hereafter referred to as just "usb-serial”) to make a serial connection between your laptop usb and the Pi's uart.

Find the usb-serial in your parts kit. One end has a USB-A connector and the other end has a 5-pin or 6-pin header. You will plug the USB-A into a usb port on your laptop and wire up jumpers from the usb-serial header pins to the uart on your Pi.
![usb-serial](../images/uart-usb-serial.jpg){: .w-50 .zoom}

The square IC in the center of the board is the CP2102 chip converts from a usb interface to a serial interface. You will need a CP2102 driver on your laptop.

> __Got CP2102 driver?__  Depending on the hardware and OS version, your laptop may have a pre-installed driver, otherwise you will need to manually install the CP2102 driver.  See [CP2102 installation guide](/guides/install/cp2102) for more info.
{: .callout-warning}

## Connect usb-serial to Pi
1. First, disconnect power to the Pi.

    >**Danger** Always have the Pi powered down whenever you are fiddling with the wiring.
    If you leave it plugged in, power is flowing and all wires are live, which makes for a dicey situation. An accidental crossed wire can a short circuit, which could fry your Pi or make your
    laptop disable the USB port.
    {: .callout-danger-invert}

2. Connect transmit, receive, and ground between the usb-serial and the Pi
    - Pick out three female-female jumpers: one blue, one green, and one black. When wiring, people experienced in electronics choose the color of the wire to indicate what type of signal is being carried.  This makes it easier to debug connections and visualize what's going on. By convention, black is used for ground, and blue and green are used for transmit and receive, respectively.
    - On the usb-serial, identify the pins labeled transmit `TX`, receive `RX`, and ground `GND`.
    - On your Mango Pi, locate the pins labeled `UART TX` and `UART RX` and any available ground pin. (Mango Pi pinout on [reference card](/guides/refcard))
    - Use the black jumper to connect ground, the blue jumper to connect the __TX__ of the usb-serial to the __UART RX__ on the Pi, and the green jumper to connect __RX__ of the usb-serial to the __UART TX__ on the Pi. The correct connections are shown in the photo below:
        ![usb-serial to Pi connections](../images/uart-connections.jpg)


>__Careful with the connections__ The connections run from one device's TX to the other's RX, i.e. the data __transmitted__ by your laptop is __received__ by your Pi and vice versa. Connecting TX to TX and RX to RX is not correct! Also note that the pins on your USB-serial
may be in different positions or have different label names. Don't just follow the photo blindly!
{: .callout-warning}

Here are some of the other usb-serial variants we have used at times (click to enlarge):
![V1](../images/uart-usb-serial-v1.jpg){: .w-25 .zoom} ![V2](../images/uart-usb-serial-v2.jpg){: .w-25 .zoom}  ![V3](../images/uart-usb-serial-v3.jpg){: .w-25 .zoom}

Once you have verified that your connections are correct, plug the usb-serial into your laptop or usb hub and then power up the Pi. You now have a serial connection!

## Commnication over tty: minicom

There are a number of programs that support communication with a tty device (`screen`, `tio`, `cutecom`, `putty`, etc.) If you don't have a favorite that you are already comfortable with, we recommend `minicom` as a simple option with nice features.

> __Got minicom?__  See guide on how to [install and configure minicom](/guides/install/minicom).
{: .callout-warning}


For uart communication to succeed, the transmitter and receiver must agree on device, baud rate, use of start, parity, and stop bits, and so on. In this course, we will be using these settings:
- peripheral `UART0`
- transmit on `GPIO PB8`, receive on `GPIO PB9`
- baud rate 115200 bps
- 1 start bit, 8 data bits, no parity bit, 1 stop bit ("8N1" for short)

<a name="troubleshooting"></a>
## Troubleshooting 

- Review all three connections to confirm correct (TX->RX, RX->TX, GND->GND).
- Reseat your jumpers if they have wiggled loose.
- If your jumper cables appear stressed or worn, replace with fresh ones.
- Confirm that your minicom settings are 115200 8N1.

