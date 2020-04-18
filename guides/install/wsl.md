---
title: Install dev tools on Windows WSL
toc: true
---

These are the instructions for setting up a development environment OS version Windows 10 using the Windows Subsystem for Linux (WSL). Take care to validate at each ✔️**Check** point before moving on to the next step. If you hit a snag, reach out on Piazza or come to office hours for help!

You may need to restart your computer a few times throughout this process, so it’s a good idea to save all work before starting so you can restart when prompted.

### Requirements

These instructions are newly updated for Winter Quarter 2020. We did our testing on **Windows 10 version 1903** and recommend that your version match ours. Confirm your OS version by typing "winver" into the start menu search bar and run the suggested command. A panel appears that reports the OS version as shown in this screenshot:

![Window Version 1903](/guides/images/winver.png)

If your OS version is 1903 (or newer), you are good to go!

{% include callout.html type='danger'%}
If you are running an earlier version of Windows; do not proceed with the installation instructions. Stop here and update to a compatible OS version first. If you need help, reach out to the staff.
</div>

### Enable WSL with Ubuntu 16.04

Windows OS does not natively support the development tools used in this course. Enabling WSL (Windows Subsystem for Linux) allows you to run an Ubuntu instance on top of Windows OS. You will install the necessary development tools into the Ubuntu WSL environment and do your coursework there.

#### Enable WSL in Settings

1. Type "features" into start bar and open the Control panel to "Turn Windows features on or off". 
1. In the panel, find the checkbox "Windows Subsystem for Linux". Check the box, and click "OK". Wait for it to complete the requested changes and then restart your computer when prompted.

#### Install Ubuntu 16.04

We will use Ubuntu version 16.04 under WSL.

{% include callout.html type='warning'%}
**NOTE**: Ubuntu 18.04 does not support all of the software requirements for the course! Make sure you select the Ubuntu 16.04 application when installing your Ubuntu system!
</div>

1. Open the Microsoft Store app. Search for "Ubuntu", select `Ubuntu 16.04 LTS` and install it.
1. Once it's done, launch the application. A terminal window will open that says "Installing, this may take a few minutes...".
1. Once that command finishes, you'll be prompted to enter a new username and password for your Ubuntu account. You’ll need this name and password for administrator privileges, so be sure to remember it!
1. You now have a linux terminal running on Windows. It's a good idea to bring the OS up-to-date, so we'll do that first. Run the following commands:

   ```
   $ sudo apt update
   $ sudo apt upgrade
   ```

✔️**Check:** confirm Ubuntu release:

```
$ lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 16.04.6 LTS
Release:        16.04
Codename:       xenial
```

You now have an up-to-date version of Ubuntu running on top of your Windows OS.

<a name="files"></a>

#### Accessing WSL files from Windows

An important thing to note about WSL is that it hosts its own file system. The files you access within the WSL terminal are separate from your regular Windows file system. You can integrate your WSL files into the Windows File Explorer by changing to a particular directory in the WSL terminal and using the command:

```
$ explorer.exe .
```

A File Explorer window opens in Windows that shows the WSL files in the current directory. Windows applications can now access these files that live in the WSL file system. This allows you to edit WSL text files using your favorite Windows text editing app. Nifty!

### Install arm-none-eabi toolchain

{% include callout.html type="warning" %}
Note: Be sure that you are in the Ubuntu shell when following the next steps to install the developer toolchain and python.
</div>
We use a cross-compiler toolchain to compile programs for the Raspberry Pi.

1. Install the gcc compiler and gdb debugger packages.

   ```
   $ sudo apt install gcc-arm-none-eabi gdb-arm-none-eabi
   ```

✔️**Check:** confirm toolchain installation with these two commands:

```
$ arm-none-eabi-gcc --version
arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529
Copyright (C) 2014 Free Software Foundation, Inc.
$ grep gdbsim `which arm-none-eabi-gdb`
Binary file /usr/bin/arm-none-eabi-gdb matches 
```

### Install Python3 and packages

The `rpi-install.py` script will be used to send programs from your computer to the Pi. This script requires installation of python3 and two support modules.

Note: Ubuntu 16.04 comes with a version of python installed, but it is an older version (version 2) which is nearing end of life. You need python version 3.

1.  Install python3 and pip. Run the following command from your WSL terminal:

    ```
    $ sudo apt install python3-pip
    ```

    ✔️**Check:** confirm `python3` installation:

    ```
    $ python3 --version
    Python 3.5.2
    ```

1.  Install the pyserial and xmodem packages for python3:

    ```
    $ python3 -m pip install pyserial xmodem
    ```

    You may see a warning about the pip version being slightly out of date, this is harmless and can be ignored.

    ✔️__Check:__ confirm `pyserial` and `xmodem` are listed as installed packages for python3:
    ```
    $ python3 -m pip list --user 
    pyserial (3.4)
    xmodem (0.4.5)
    ```

### Install CP2012 console driver

The console driver enables the bootloader client to communicate with the Pi over the USB-serial device. Note: this installation step is done from Windows (not inside the WSL shell).

1. Download the CP2012 driver. The drivers are available on the Silicon Labs [CP210x Downloads page](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers). Scroll down to the section titled "Download for Windows 10 Universal" and download the VCP zip file. Decompress the zip file and open the containing folder.
1. Find the file named `CP210xVCPInstaller_x64.exe` (if your system is 64-bit) or `CP210xVCPInstaller_x86.exe` (if 32-bit). If you don't know if your system is 32 or 64-bit, see [these instructions](https://support.microsoft.com/en-us/help/13443/windows-which-version-am-i-running). Open the appropriate installer file and follow the prompts.

✔️**Check:**  Confirm the driver appears in the list of system drivers by following the steps below:

1. Open the "System Information" app. A panel pops up with a "System Summary" of your hardware and software environment.
1. Use the "Find what:" field at the bottom of the panel to search for "silabs". (Be patient while it searches.)
1. The search should report a matching Serial Port Driver with name "SILABSER.SYS" and description "Silicon Labs CP210x USB to UART Bridge Driver".

{% include callout.html type="danger" %}
We are not sure the driver check step is reliable, as it may be that the driver will not show up in the list unless loaded. You won't be able to test loading the driver until you get your kit in lab1, so hang tight for now. Keep an eye on Piazza and we'll make a post if we discover a reliable way to confirm driver installation without loading it.
</div>

{% comment %}
Sean April 2020, We had a few students that were having trouble with the driver, and were getting a "could not find CP2102 device" error. I'm adding the driver install instructions back now since the redundancy doesn't seem to break anything.

Julie Jan 2020,Windows 10 should be plug-and-play, will automatically download/install driver. I tested on brand new machine and confirmed it worked as advertised. Commenting out the instructions for manual install, but will leave here in case we later find we need to revive them
{% endcomment %}

### Finished
After following the above steps, you've installed the development tools. Continue on to follow the instructions to [configure your user account](../#steps).
