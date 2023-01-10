---
title: 'Guide: Install on macOS'
toc: true
---

<script>
$().ready(function() {
    var elems = document.getElementsByClassName('language-console');
    for (const elem of elems) elem.className += ' console-mac';
});
</script>

These are the instructions for installing the development tools on macOS. We peppered the installation instructions with <i class="fa fa-check-square-o fa-lg"></i> __Check__ steps that confirm your progress through the steps. Use each to validate a step before moving on.  If you hit a snag, stop and reach out (forum, office hours) and we can help you out!

## Install Xcode

To ease the installation process, we are using Homebrew.
[Homebrew](http://brew.sh/) is a popular [package
manager](https://en.wikipedia.org/wiki/Package_manager) for OS X.

If you already use Homebrew, skip to the next section; otherwise follow these steps.

1.  Homebrew requires the Xcode command line tools. Install these by running the command below.
    ```console
    $ xcode-select --install
    ```

2.  Install Homebrew by running the command below.
    ```console
    $ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    ```

{% include checkstep.html content="confirm Homebrew installation" %}
```console
$ brew -v
Homebrew 3.3.3
```
Fine if your version number is newer.

## Install arm-none-eabi toolchain
We use a cross-compiler toolchain to compile programs for the Raspberry Pi. 

1. Install our custom brew formula containing the cross-compile tools.
    ```console
    $ brew install cs107e/cs107e/arm-none-eabi-test
    ```

{% include checkstep.html content="confirm compiler" %}
```console
$ brew info arm-none-eabi-test
cs107e/cs107e/arm-none-eabi-test: stable 9-2019q4-cs107e
ARM embedded toolchain MacOS for cs107e
$ arm-none-eabi-gcc --version
arm-none-eabi-gcc (GNU Tools for Arm Embedded Processors 9-2019-q4-major) 9.2.1 20191025 (release) [ARM/arm-9-branch revision 277599]
```

## Install python3 and packages
The `rpi-run.py` script will be used to send programs from your computer to the Pi. This script requires installation of python3 and two support modules.

Note: macOs have a version of python pre-installed, but it is an older version (version 2) which is nearing end of life. You need python version 3. If you previously have installed python3, skip step 1 below.

1. Install python3. Python installers are available from <https://www.python.org/downloads/mac-osx/>. Choose the latest stable Python 3 release and download its macOS 64-bit installer. Open the downloaded pkg file and follow the instructions to install python3.

2. Install the pyserial and xmodem packages for python3:

    ```console
    $ python3 -m pip install pyserial xmodem
    ```

{% include checkstep.html content="confirm python3 and packages pyserial and xmodem" %}
```console
$ python3 --version
python 3.9.1
$ python3 -m pip show pyserial xmodem 
Name: pyserial
Version: 3.5
...
---
Name: xmodem
Version: 0.4.6
...
```
Your versions may be slightly newer (higher numbers). That's fine!

## Install CP2012 console driver
The console driver enables the bootloader client to communicate with the Pi over the USB-serial device.

> __Is your macOS version Big Sur, Monterey or newer?__ Check "About This Mac" to confirm. If your macOS version is >= 11.16, you should have CP2102 support out of the box. Skip over this section and do not install the Silicon Labs CP2102 driver. 
{: .callout-warning}

1. Download the CP2012 driver. The drivers are available on the Silicon Labs [CP210x Downloads page](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers). Select the "Downloads" tab and find the link for the "CP210x VCP Mac OSX Driver" and download the zip file. Open the zip to decompress into the dmg file `SiLabsUSBDriverDisk.dmg`. Open the dmg file to mount the volume named `Silicon Labs VCP Driver Install Disk`.

2. On the mounted volume, find the file named
    `Install CP2010x VCP Driver`. Open this file to launch the
    installer. Follow its instructions to install the driver.

3. If your system raises a "System Extension Blocked" alert, you must take additional action to enable the driver. Open `System Preferences` and choose the `Security & Privacy` option. On the `General` tab, find the message indicating that "Silicon Laboratories Inc" has been blocked and click `Allow` to unblock it. Restart your computer for this change to take effect.

{% include checkstep.html content="confirm CP2102 driver is installed and loadable" %}
```console
$ kextfind -B -si silabs -report -b -loadable
CFBundleIdentifier  Loadable
com.silabs.driver.CP210xVCPDriver   yes
```
{% include_relative userconfig.md %}
