---
layout: page
title: Mac Installation Guide
---

For this class, you will need to install the following software:

1.  The `arm-none-eabi` toolchain, which includes a C x86 to ARM cross-compiler
    which compiles C code on your x86 machine to ARM machine code which can run
    on the Raspberry Pi.
2.  The driver and script for the CP2102 console cable which connects your
    computer and Pi, so you can load programs onto the Pi.

If you are on a Mac, follow all the instructions on this page.

### `arm-none-eabi` installation

To ease the installation process for the `arm-none-eabi` tools on OS X, we have
built and packaged them into a Homebrew formula for you.
[Homebrew](http://brew.sh/) is a popular [package
manager](https://en.wikipedia.org/wiki/Package_manager) for OS X. It is good
practice to install software using a package manager.

#### Install Homebrew

If you aren't yet using Homebrew, use these instruction to get Homebrew up and
running. If you are already using Homebrew, skip to the next section.

1.  Install the Xcode command line tools by running the command below. A dialog
    should pop up on your screen. Click "Install" when it appears.

    ```
$ xcode-select --install
    ```

2.  Run Homebrew's install script by using the command below. The script will
    guide you through the rest of the install process.

    ```
$ ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
    ```

#### Install the `arm-none-eabi` formula

1.  "Tap" into the CS107e Homebrew packages repository using the command below.
    This allows you to use the Homebrew formulas we have provided.

    ```
$ brew tap cs107e/cs107e
    ```

2.  Install the `arm-none-eabi` toolchain:

    ```
$ brew install arm-none-eabi
    ```

3.  Ensure it works by running the following command and checking that its
    output is identical to what is displayed below:

    ```
$ arm-none-eabi-gcc --version
arm-none-eabi-gcc (GCC) 4.8.3
Copyright (C) 2013 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    ```

### Using the CP2120 Console Cable

To use the CP2102 console cable, you will need to install the drivers.
Instructions on how to do this are in the [Console Guide](../console.md). Also
download the [rpi-install.py
script](https://raw.githubusercontent.com/cs107e/courseware/master/guides/mac_toolchain/rpi-install.py),
which you will be using to load your programs onto the Pi over the serial
cable.

If you try to run this script, you may run across errors that there is "No
module named serial" or "No module named xmodem". You will need to install
these Python modules. If you do not have pip (Python package management) on
your Mac, [download and install
pip](https://pip.pypa.io/en/latest/installing.html). Once you have pip, you can
easily get the missing modules using

    $ sudo pip install pyserial
    $ sudo pip install xmodem

Once those are installed, you should be able to run the script. You
will probably want to add the path of wherever you put the script to
your PATH variable (see above) so that you can run it from any
directory. The [bootloader guide](../bootloader.md) will go over how to
load programs over the console cable.
