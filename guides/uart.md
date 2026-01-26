---
title: "Guide: Serial communication over uart"
toc: true
attribution: Written by Julie Zelenski
---
## What is uart?

UART is an acronym for __Universal Asynchronous Receiver-Transmitter__, a protocol for sequential serial communication.  Most processors include hardware support for a uart peripheral (the Mango Pi has six of them in fact!). A uart is a serial communication channel for sending and receiving data between devices. Your laptop will connects to the Pi's uart to receive output and provide input.

## USB-serial breakout board

We use a CP2102 USB-to-serial breakout board (hereafter referred to as "usb-serial”) to make a serial connection between your laptop usb and the Pi's uart.

Find the usb-serial in your parts kit. One end has a USB-A connector and the other is a 5-pin or 6-pin header. You will plug the USB-A end into a usb port and connect jumpers from the header end to the Pi.
![usb-serial](../images/uart-usb-serial.jpg){: .w-50 .zoom}

The usb-serial requires a CP2102 driver installed on your laptop. Recent macOS versions ship with a driver pre-installed. If you are using Windows/WSL, you must manually install the driver.

<blockquote class="callout-warning" markdown="1">
__Got CP2102 driver?__  If running macOS, you already have a CP2102 driver out of the box. Skip over this section; do not manually install the driver.
<details markdown="1">
<summary>Expand for instructions to manually install CP2012 driver on Windows/WSL</summary>
1. Only use these instructions for Windows v10 or v11.
1. Installing the CP2102 driver is done from Windows (not inside the WSL terminal). Switch to your Windows web browser and go to the Silicon Labs [CP210x Downloads page](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads).
1. Select the "Downloads" tab and find "CP210x Windows Drivers v6.7.6". Download this zip file.
    - __Be sure to choose version 6.7.6!__ Don't be confused by other drivers with similar names and slightly different version numbers. The version to download is exactly __CP210x Windows Drivers v6.7.6__.
1. Extract all files from the downloaded zip file. Look in the uncompressed folder for the installer exe file that matches the architecture of your laptop.  If your laptop is 64-bit, the installer exe is named `CP210xVCPInstaller_x64.exe`. If 32-bit, it is `CP210xVCPInstaller_x86.exe`.
    - If you are not sure whether your laptop is 32 or 64-bit, use command `dpkg --print-architecture` in your WSL terminal and look for a response of `amd64` (64-bit) or `i386` (32-bit).
1. Run the installer exe file and follow its instructions.
</details>
</blockquote>

<A name="find-dev"></A>
## Find device name
When the usb-serial is plugged into your laptop, it appears as a new entry in the filesystem, with a name of the form `/dev/YOUR_DEVICE_NAME` for some value of __YOUR_DEVICE_NAME__. You'll need to know the device name on your system in order to connect to it.

How to get the device name on your system:
1. Try `find-dev.sh`
    This is a little script we hacked up that attempts to ferret out the usb-serial device name. It works for some setups but not all.

    When `find-dev.sh` finds the usb-serial, it prints the device name. If it doesn't find your usb-serial, double-check that it is connected and powered on. If `find-dev.sh` just isn't working for you, move on to method #2 below.

    ```console
    $ find-dev.sh
    /dev/cu.usbserial-0001

    $ find-dev.sh
    Could not find CP2102 serial device.
    I looked through the serial devices on this computer, and did not
    find a device associated with a CP2102 USB-serial adapter. Is
    your USB-serial adapter plugged in?
    ```

1.  Instead use your shell ninja powers to suss out the device name.
    The idea is to capture a list of the files in directory `/dev/tty*` when the usb-serial is unplugged and take another capture after connecting it. Applying `diff` to the two listings will show what changed, which gives you the device name for the usb-serial. Knowing your way around the unix command-line can be very helpful!

    ```console?prompt=$
    $ ls /dev/* >unplugged     # capture ls output into file named unplugged
    $ ls /dev/* >plugged       # capture again after plugging in usb-serial
    $ diff plugged unplugged
    2a3
    > /dev/ttyS3
    $ rm plugged unplugged        # remove temp files
    ```

Note that the device name on __your__ system may be different than the examples above.
If you are not able to find the device name, ask us for help!

## Connect usb-serial to Pi
1. First, disconnect power to the Pi.

    >__Danger__ Always have the Pi powered down whenever you are fiddling with the wiring.
    If you leave it plugged in, power is flowing and all wires are live, which makes for a dicey situation. An accidental crossed wire can a short circuit, which could fry your Pi or make your
    laptop disable the USB port.
    {: .callout-danger-invert}

2. Connect transmit, receive, and ground between the usb-serial and the Pi
    - Pick out three female-female jumpers: one blue, one green, and one black. People experienced in electronics choose the color of the wire to indicate what type of signal is being carried.  This makes it easier to debug connections and visualize the circuit. By convention, black is used for ground, and blue and green are used for transmit and receive, respectively.
    - Look at your usb-serial and identify the pins labeled transmit `TX`, receive `RX`, and ground `GND`. Connect the jumpers to the usb-serial as follows:
        - blue: usb-serial __TX__
        - green: usb-serial __RX__
        - black: usb-serial __GND__
        ![jumpers connected to usb-serial](../images/usb-serial-connect.png){: .w-75 .zoom}
    - Use the Mango Pi [pinout](/guides/refcard) to locate the header pins for `UART TX` and `UART RX`. Also find an open ground pin. Connect the jumpers from usb-serial to the Mango Pi as follows
        - blue: usb-serial __TX__ <-> Pi __UART RX__
        - green: usb-serial __RX__ <-> Pi __UART TX__
        - black: usb-serial __GND__ <-> Pi __ground__
        ![usb-serial to Pi connections](../images/uart-usb-serial-connect.jpg){: .w-50 .zoom}
    We recommend the arrangement shown above: OTG connected to end of hub and usb-serial connected to side of hub (this configuration leaves open access to the Pi's other USB/HDMI ports).

>__Careful with the connections__ The connections run from one device's TX to the other's RX, i.e. the data __transmitted__ by your laptop is __received__ by your Pi and vice versa. Connecting TX to TX and RX to RX is not correct!
{: .callout-warning}

The usb-serial in your parts kit may be a bit different than photos above, so be sure to read the labels rather than follow the photos blindly. Here are a few examples of usb-serial variants we have used (click to enlarge):
![V1](../images/uart-usb-serial-v1.jpg){: .w-25 .zoom} ![V2](../images/uart-usb-serial-v2.jpg){: .w-25 .zoom}  ![V3](../images/uart-usb-serial-v3.jpg){: .w-25 .zoom}

Once you have verified that your connections are correct, plug the usb-serial into your laptop or usb hub and then power up the Pi. You now have a serial connection!

## Commnication over tty: tio

There are a number of programs that support communication with a tty device (`screen`, `minicom`, `putty`, etc.) Our favorite of the bunch is `tio` for its simple interface and nice features, so this is our strong recommendation. (If you already very comfortable with another program, ok to keep using instead).

> __Got tio?__  See guide on how to [install and configure tio](/guides/install/tio).
{: .callout-warning}


A successful uart connection requires that the transmitter and receiver agree on all settings: device name, baud rate, use of start, parity, and stop bits. We are using these settings for communicating between your laptop and Mango Pi:
- peripheral `UART0`
- transmit on `GPIO PB8`, receive on `GPIO PB9`
- baud rate 115200 bps
- 1 start bit, 8 data bits, no parity bit, 1 stop bit ("8N1" for short)

<a name="troubleshooting"></a>
## Troubleshooting 
- Review all three connections to confirm correct (TX->RX, RX->TX, GND->GND).
- Reseat your jumpers if they have wiggled loose.
- If jumper cables appear stressed or worn, replace with fresh ones.
- Confirm that your settings are 115200 8N1.
- If you try to connect and receive the error `Device file is locked by another process`, this typically means that `tio` is already running and connected to the device. Look through your windows to find your existing connnection instead of trying to start another one.
- In some situations, the usb-serial device name can change from what it was previously. Re-do the [hunt for the device name](#find-dev).
- If tio gets a "permission denied" error when trying to connect to your device, try adding your user id to the group of the `/dev` files (possibly `tty` or `dialout`). Ask a staff member for help.


