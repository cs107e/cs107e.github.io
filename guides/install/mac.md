---
title: Installing dev tools on MacOS
toc: true
---

These are the instructions for installing the development tools on MacOS. Take care to validate at each ✔️__Check__ point before moving on to the next step.  If you hit a snag, reach out on Piazza or come to office hours for help!

### Install arm-none-eabi toolchain

#### Install Homebrew

To ease the installation process, we packaged the toolchain into a Homebrew formula.
[Homebrew](http://brew.sh/) is a popular [package
manager](https://en.wikipedia.org/wiki/Package_manager) for OS X.

If you are already using Homebrew, skip to the next section ; otherwise follow these steps.

1.  Install the Xcode command line tools by running the command below and following its instructions.

    ```
    $ xcode-select --install
    ```

2.  Install Homebrew by running the command below and following its instructions.
    ```
    $ ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
    ```

✔️__Check:__ confirm Homebrew installation: (okay if your version is newer)

```
$ brew -v
Homebrew 2.2.1
Homebrew/homebrew-core (git revision 8eed; last commit 2019-12-19)
```

#### Install `arm-none-eabi` formula

1. "Tap" into the CS107e Homebrew package. 

    ```
    $ brew tap cs107e/cs107e
    ```

2.  Install the `arm-none-eabi` formula.

    ```
    $ brew install arm-none-eabi
    ```

✔️__Check:__ confirm toolchain installation:

```
$ arm-none-eabi-gcc --version
arm-none-eabi-gcc (GCC) 4.8.3
Copyright (C) 2013 Free Software Foundation, Inc.
```

### Install CP2012 console driver

The console driver enables the bootloader client to communicate with the Pi over the USB-serial device.

1. Download the CP2012 driver. The drivers are available on the Silicon Labs [CP210x Downloads page](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers). Scroll down to the section titled "Download for Mac OS" and download the VCP zip file. Open the zip to decompress into the dmg file `SiLabsUSBDriverDisk.dmg`. Open the dmg file to mount the volume named `Silicon Labs VCP Driver Install Disk`.

2. On the mounted volume, find the file named
    `Install CP2010x VCP Driver`. Open this file to launch the
    installer. Follow its instructions to install the driver.

{% include callout.html type='warning'%}
**NOTE**: You may get an warning saying "System Extension Blocked" depending on your security settings. If you see this message, you must take additional action to enable the driver. Open `System Preferences` and choose the `Security & Privacy` option. On the `General` tab, find the message that the developer "Silicon Laboratories Inc" has been blocked and click `Allow` to unblock it. You must restart your computer for this change to take effect.
</div>

✔️__Check:__ confirm CP2102 driver is installed and loadable:

```
$ kextfind -B -s SiLabs -report -b -loadable
CFBundleIdentifier  Loadable
com.silabs.driver.CP210xVCPDriver   yes
```

### Install python3 and packages
The `rpi-install.py` script will be used to send programs from your computer to the Pi. This script requires installation of python3 and two support modules.

Note: Macs come with a version of python installed, but it is an older version (version 2) which is nearing end of life. You need python version 3. If you previously have installed python3, skip step 1 below.

1. Install python3. Python installers are available from <https://www.python.org/downloads/mac-osx/>. Choose the latest stable Python 3 release and download its macOS 64-bit installer. Open the downloaded pkg file and follow the instructions to install python3.

    ✔️__Check:__  confirm `python3` installation:
    ```
    $ python3 --version
    Python 3.8.1
    ```

3. Install the pyserial and xmodem packages for python3:

    ```
    $ python3 -m pip install pyserial xmodem
    ```

    You may see a warning about the pip version being slightly out of date, this is harmless and can be ignored.

    ✔️__Check:__ confirm `pyserial` and `xmodem` are listed as installed packages for python3:
    ```
    $ python3 -m pip list 
    Package    Version
    ---------- -------
    pyserial   3.4    
    xmodem     0.4.5 
    ```

### Finished
After following the above steps, you've installed the development tools. Continue on to follow the instructions to [configure your user account](../#steps).

