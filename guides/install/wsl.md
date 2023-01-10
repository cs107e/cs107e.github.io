---
title: 'Guide: Install on Windows WSL'
toc: true
---

<script>
$().ready(function() {
    var elems = document.getElementsByClassName('language-console');
    for (const elem of elems) elem.className += ' console-ubuntu';
});
</script>

These are the instructions for setting up a development environment using the Windows Subsystem for Linux (WSL). We peppered the installation instructions with <i class="fa fa-check-square-o fa-lg"></i> __Check__ steps that confirm your progress through the steps. Use each to validate a step before moving on.  If you hit a snag, stop and reach out (forum, office hours) and we can help you out!

You may need to restart your computer a few times throughout this process, so it’s a good idea to save all work before starting so you can restart when prompted.

## Requirements
Confirm that you are running an appropriate version of Windows 10. To find the version number, type `winver` into the start menu search bar and run the command. A panel appears that reports the version information as shown in this screenshot:

![Window Version 1909](images/winver.png)
{: .w-75}

When we updated our installation instructions for Winter Quarter 2021, we tested on __Version 1909__ of Windows 10 and recommend that your version match ours. 
> __Windows version earlier than 1909__ If you are running a Windows version that is earlier 1909; you should not proceed with the installation instructions. Stop here and update to a compatible OS version first. If you need help, reach out to the staff.
{: .callout-danger-invert}

## Enable WSL and install Ubuntu 20.04

Windows OS does not natively support the development tools used in this course. Enabling WSL (Windows Subsystem for Linux) allows you to run an Ubuntu instance on top of Windows OS. You will install the necessary development tools into the Ubuntu WSL environment and do your coursework there.

### Enable WSL Windows feature

1. Type "features" into start bar and open the Control panel to "Turn Windows features on or off". 
1. In the panel, find the checkbox "Windows Subsystem for Linux". Check the box, and click "OK". Wait for it to complete the requested changes and then restart your computer when prompted.

### Install Ubuntu 20.04

Ubuntu version 20.04 LTS for WSL is available in the Microsoft Store.

1. Open the Microsoft Store app. Search for "Ubuntu 20", select `Ubuntu 20.04 LTS` and install it.
    > __Be sure to choose version Ubuntu 20.04 LTS__
    > The other Ubuntu versions do not support the software requirements we need.
    {: .callout-warning}
1. Once it's done, launch the application. A terminal window will open that says "Installing, this may take a few minutes...".
1. Once that command finishes, it prompts you to enter a new username and password for your Ubuntu account. __Be sure to remember the name and password__, you'll need them later for administrator privileges.
1. Apply the latest Ubuntu updates by running the command below in your WSL terminal window:

    ```console
    $ sudo apt update && sudo apt upgrade
    ```
Do not be alarmed about the long-winded unix-babble produced -- these processes are total chatterboxes.

{% include checkstep.html content="confirm Ubuntu and WSL version" %}
```console
$ lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 20.04.1 LTS
Release:        20.04
Codename:       focal
$ powershell.exe "wsl --list --verbose"
  NAME              STATE       VERSION
  Ubuntu-20.04      Running     1
```
The rightmost column of the last command reports the version of WSL. It is critical that you are using WSL version 1, not 2, as serial port support is not available in WSL2. 

You now have an up-to-date version of Ubuntu running in WSL1 on top of your Windows OS.

<a name="files"></a>
### Accessing WSL files from Windows

An important detail to note about WSL is that it hosts its own file system. The files you access within the WSL terminal are separate from your regular Windows file system. You can access the WSL files in the Windows File Explorer by changing to a particular directory in the WSL terminal and run the command below:

```console
$ explorer.exe .
```

This command opens a window in File Explorer that shows the files in the current WSL directory (whose shortname name is dot). Windows applications can now access these files, such as to edit a WSL text file using your favorite Windows text editor. Nifty!

## Install arm-none-eabi toolchain

We use a cross-compiler toolchain to compile programs for the Raspberry Pi. Run the commands below in your WSL terminal to install the toolchain.

1. Download our toolchain package file:
    ```console
    $ wget https://github.com/cs107e/homebrew-cs107e/raw/master/gdb-arm-none-eabi_9.2-0ubuntu1_20.04+10-107e_amd64.deb
    ```

2. Install the package.

   ```console
   $ sudo apt install -f ./gdb-arm-none-eabi_9.2-0ubuntu1_20.04+10-107e_amd64.deb
   ```


{% include checkstep.html content="confirm compiler" %}
```console
$ apt list gdb-arm-none-eabi
gdb-arm-none-eabi/now 9.2-0ubuntu1~20.04+10-107e amd64 [installed, local]
$ arm-none-eabi-gcc --version
arm-none-eabi-gcc (15:9-20190q4-0ubuntu1) 9.2.1 20191024 [ARM/arm-9-branch revision]
```

## Install Python3 and packages

The `rpi-run.py` script will be used to send programs from your computer to the Pi. This script uses python3 and two support packages. Run the commands below in your WSL terminal to install these components.

1.  Install python3-pip.

    ```console
    $ sudo apt install python3-pip
    ```


1.  Install the pyserial and xmodem packages.

    ```console
    $ python3 -m pip install pyserial xmodem
    ```

    You may see a warning about the pip version being slightly out of date, this is harmless and can be ignored.

{% include checkstep.html content="confirm python3 and packages pyserial and xmodem" %}
```console
$ python3 --version
Python 3.8.5
$ python3 -m pip show pyserial xmodem 
Name: pyserial
Version: 3.4
... blah blah blah ...
Name: xmodem
Version: 0.4.6
... blah blah blah ...
```
Your versions may be slightly newer (higher numbers). That's fine!

## Install CP2012 console driver
The console driver enables the bootloader client to communicate with the Pi over the USB-serial device. 

1. The installation of the CP2102 driver is done from Windows (not inside the WSL terminal). Switch to your Windows web browser and go to the Silicon Labs [CP210x Downloads page](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers).
1. On this page, select the "Downloads" tab and find the link for "CP210x Windows Drivers v6.7.6". Download this zip file. 
    > __Be sure to choose version 6.7.6!__ Don't be confused by other drivers with similar names and slightly different version numbers. The version to download is exactly __CP210x Windows Drivers v6.7.6__. 
    {: .callout-warning}
1. Extract all files from the downloaded zip file. Look in the uncompressed folder for the installer exe file that matches the architecture of your laptop.  If your laptop is 64-bit, the installer exe is named `CP210xVCPInstaller_x64.exe`. If 32-bit, it is `CP210xVCPInstaller_x86.exe`.
    - If you don't know whether your laptop is 32 or 64-bit, use command `dpkg --print-architecture` in your WSL terminal and look for a response of `amd64` (64-bit) or `i386` (32-bit).
{%- comment %}
REMINDER: newer universal driver not compatible with WSL, keep using the older 6.7.6 version! https://github.com/cs107e/staff/issues/225
{%- endcomment %}
1. Run the appropriate installer exe and follow the prompts to completion.

{% comment %}
Removed this check step as driver does not show up until it has been loaded. Leave this information here for trouble-shooting.

{% include checkstep.html content="confirm System Information finds driver" %}
1. Open the "System Information" app. A panel pops up with a "System Summary" of your hardware and software environment.
1. Use the "Find what:" field at the bottom of the panel to search for "silabs". (Be patient while it searches)
1. The search should find matching Serial Port Driver with name "SILABSER.SYS" and description "Silicon Labs CP210x USB to UART Bridge Driver".

    > __Driver not found?__ We suspect that the Windows System Information may not find the driver if it has net yet loaded it for a device, so it doesn't find the driver yet, don't be alarmed. You can confirm later when you have the USB-serial device to test the driver.
    {: .callout-warning}
{% endcomment %}

{% comment %}
Sean April 2020, We had a few students that were having trouble with the driver, and were getting a "could not find CP2102 device" error. I'm adding the driver install instructions back now since the redundancy doesn't seem to break anything.

Julie Jan 2020,Windows 10 should be plug-and-play, will automatically download/install driver. I tested on brand new machine and confirmed it worked as advertised. Commenting out the above instructions for manual install from SiLabs, but will leave here in case we later find we need to revive them
{% endcomment %}

{% include_relative userconfig.md %}
