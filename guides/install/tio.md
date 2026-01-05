---
title: 'Guide: Install Tio'
toc: true
---

## What is Tio?

To communicate with your Pi over the USB-serial, you will need to run a terminal communication program on your laptop that can interface with a tty device. There are a variety of communication programs that can work: `screen`, `minicom`, `putty`, and `tio`, among others.  The `screen` program is the lowest common denominator; its one advantage is that is  universally available without having to install anything extra, but it provides only the most basic functionality and has a beastly UI.  We think you'll do better with something more user-friendly. Our favorite of the bunch is `tio` for its simple interface and nice features, so this is our strong recommendation. (If you already very comfortable with another communication program, ok to keep using instead).

## Install tio
Use your package manager to install `tio`

-  On __macOS__:
    ```console
    $ brew install tio
    ```
- On __WSL/Ubuntu__:
    ```console
    $ sudo apt install tio
    ```

## Documentation on tio
The tio maintainer has a lovely github page introducing tio and its features [https://github.com/tio/tio](https://github.com/tio/tio?tab=readme-ov-file#tio---a-serial-device-io-tool)

## Configure tio

You invoke tio with the device to connect to and various flags to configure the communication settings:

```console 
$ tio -b 115200 -d 8 -s 1 -p none  /dev/YOUR_DEVICE_NAME
```

The above command connects to a device and sets the baud rate to 115200 baud, 8 data bits, 1 stop bit, and no parity. Be sure to replace __YOUR_DEVICE_NAME__ with the name of CP2102 device on your system. See [instructions in uart guide](/guides/uart#find-dev) for finding your device name.

To exit tio, use the sequence `control-t q`.

Rather than re-type all these flags each time, you can store these settings in your tio configuration file and then retrieve them by name. Create a new file `~/.tioconfig`. Open the file in your editor and paste in the text below which defines a new configuration named `mango`.  Be sure to replace __YOUR_DEVICE_NAME__ with actual device name on your system.

```
[mango]
device = /dev/YOUR_DEVICE_NAME
baudrate = 115200
databits = 8
parity = none
stopbits = 1
```

Save the file and exit your editor. Now when you invoke `tio mango`, tio will look in your `.tioconfig` file to use the settings for `mango`, no additional flags needed!

## Do a loopback test

Grab your USB-serial adapter and a female-female jumper to test out tio now. Connect TX to RX on the USB-serial adapter in loopback mode, as shown in this photo:

![CP2102 loopback](/labs/lab3/images/loopback.jpg){: .zoom}

Start tio on your device and type characters in the tio window. The characters should be echoed back.
```console
$ tio /dev/YOUR_DEVICE_NAME
```
In loopback mode, the signals sent out on the TX pin are wired straight to the RX pin. Reading from the RX pin will read the characters sent over TX.

When you connect the TX and RX to the Mango Pi, tio is bridging the communication from your laptop to the Pi and back. Tio sends what you type to the Pi and displays what is received from the Pi. Neat!

## Connect and disconnect

When you disconnect or reset your Pi (by unplugging or flipping the switch), an open tio connection will pause. It will print a status message  `Disconnected, Waiting for tty device` to let you know the connection is currently paused. When the Pi is ready again, tio will automatically reconnect and resume communication.

> __Pro-tip__
Open a separate window/tab to run `tio` and always keep it running. No need to exit and restart `tio`. When you disconnect or reset your Pi, it will simply pause (prints "Disconnected, Waiting for tty device"). When your Pi resets, tio will automatically reconnect and resume communication.
{: .callout-info}

## Other handy info
- If you try to connect and receive the error `Device file is locked by another process`, this typically means that `tio` is already running and connected to the device. Look through your windows to find your existing connection instead of trying to start another one.
- I like to set a distinctive background on terminal window that is running `tio` (mango orange, yeah!). This makes my tio window easier to spot. Set the background color of a macOS terminal via `Edit Background Color` in Shell menu (or `Show Inspector` for additional customization options). In WSL, you can set tab color by right-clicking on title and choose `Change tab color` from menu.
- On Windows, your settings may be configured to play an alert sound each time a USB device connects or disconnects. If you find this irksome, you can disable through the setting `Sounds` -> `Program Events` -> `Device Connect/Device Disconnect`, set sound to `none`.

