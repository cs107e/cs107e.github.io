---
title: Install dev tools on Windows WSL
toc: true
---

These are the instructions for setting up a development environment OS version Windows 10 using the Windows Subsystem for Linux (WSL). Take care to validate at each ✔️**Check** point before moving on to the next step. If you hit a snag, reach out on Piazza or come to office hours for help!

You may need to restart your computer a few times throughout this process, so it’s a good idea to save all work before starting so you can restart when prompted.

### Requirements

{% include callout.html type='danger'%}

These instructions are newly updated for Winter quarter 2020. We did our testing on **Windows 10 version 1903** and recommend that your version match ours. Do not follow these instructions on an earlier version of Windows; instead reach out to staff for help setting up your environment.

</div>

### Enable WSL with Ubuntu 16.04

Windows OS does not natively support the development tools used in this course. Enabling WSL (Windows Subsystem for Linux) allows you to run an Ubuntu instance on top of your Windows OS. You will install the necessary development tools into the Ubuntu WSL environment and do your coursework there.

#### Enable WSL in Settings

1. Open the Settings app and navigate to Apps -> Apps & Features
2. Scroll to the bottom of the page, and click the link that says "Programs and Features". A new window will open with a list of applications installed on your machine
3. On the left side, click the link titled "Turn Windows features on or off". Another window should open with a list of checkboxes and titles
4. Find the checkbox for "Windows Subsystem for Linux". Check it, and click "OK". The message "Applying changes" displays. Wait for it to finish. Once it's finished, restart your computer.

#### Install Ubuntu 16.04

We will use Ubuntu version 16.04 under WSL.

1. Open the Windows Store app and type "Ubuntu" in the search bar.
2. Select the result titled "Ubuntu 16.04 LTS" and click Install on the app landing page.
3. Once it's done installing, launch the application. A terminal window will open that says "Installing, this may take a few minutes...".
4. Once it's done installing, you'll be prompted to enter a new username and password for your Ubuntu account. You’ll need this name and password for administrator privileges, so be sure to remember it!
5. Once you're done entering your username and password, you have a normal linux terminal to work in. It's a good idea to bring the OS up-to-date, so we'll do that first. Run the following commands:

   ```
   $ sudo apt update
   $ sudo apt upgrade
   ```

You now have an up-to-date version of Ubuntu running on top of your Windows OS.

<a name="files"></a>

#### Accessing WSL files from Windows

An important thing to note about WSL is that it hosts its own file system. The files you access within the WSL terminal are separate from your regular Windows file system. You can integrate your WSL files into the Windows File Explorer by changing to a particular directory in the WSL terminal and using the command:

```
$ explorer.exe .
```

A File Explorer window opens in Windows that show the WSL files in the current directory. Windows applications can now access these files that live in the WSL file system. This allows you to edit WSL text files using your favorite Windows text editing app. Nifty!

### Install arm-none-eabi toolchain

{% include callout.html type="warning" %}
Note: Be sure that you are in the Ubuntu shell when following the next steps (installing toolchain and python).

</div>
This is the toolchain we need in order to compile code for the Raspberry Pi.

1. Install the gcc compiler

   ```
   $ sudo apt install gcc-arm-none-eabi
   ```

2. Install the gdb debugger
   ```
   $ sudo apt install gdb-arm-none-eabi
   ```

✔️**Check:** confirm toolchain installation:

```
$ arm-none-eabi-gcc --version
arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529
Copyright (C) 2014 Free Software Foundation, Inc.
```

### Install Python3 and packages

The `rpi-install.py` script will be used to send programs from your computer to the Pi. This script requires installation of python3 and two support modules.

Note: Ubuntu 16.04 comes with a version of python installed, but it is an older version (version 2) which is nearing end of life. You need python version 3.

1.  Install python3. Run the following command from your WSL terminal:

    ```
    $ sudo apt install python3
    ```

    ✔️**Check:** confirm `python3` installation:

    ```
    $ python3 --version
    Python 3.5.2
    ```

2.  Install pip3 for python3

    ```
    $ sudo apt-get install python3-pip
    ```

    ✔️**Check:** confirm `pip3` installation:

    ```
    $ pip3 --version
    pip 8.1.1 from /usr/lib/python3/dist-packages (python 3.5)
    ```

3.  Install the pyserial and xmodem packages for python3:

    ```
    $ pip3 install pyserial xmodem
    ```

    You may see a warning about the pip version being slightly out of date, this is harmless and can be ignored.

    ✔️**Check:** confirm the packages are installed by using import command within python3 as shown below:

    ```
    $ python3
    Python 3.8.1 (v3.8.1:1b293b6006, Dec 18 2019, 14:08:53)
    [Clang 6.0 (clang-600.0.57)] on darwin
    Type "help", "copyright", "credits" or "license" for more information.
    >>> import serial, xmodem
    ```

    If there is a problem accessing the modules, you will see an error; otherwise, the expected response from a successful import is simply silence.

### Install CP2012 console driver

The console driver enables the bootloader client to communicate with the Pi over the USB-serial device. Note: this installation step is done from Windows (not inside the WSL shell).

1. Download the CP2012 driver. The drivers are available on the Silicon Labs [CP210x Downloads page](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers). Scroll down to the section titled "Download for Windows 10 Universal" and download the VCP zip file. Decompress the zip file and open the containing folder.
2. Find the file named `CP210xVCPInstaller_x64.exe` (if your system is 64-bit) or `CP210xVCPInstaller_x86.exe` (if 32-bit). If you don't know if your system is 32 or 64-bit, see [these instructions](https://support.microsoft.com/en-us/help/13443/windows-which-version-am-i-running). Open the appropriate installer file and follow the prompts.

   ✔️**Check:** Confirm the driver is installed by validating you see the driver in the list of drivers after following these steps:

   1. Type "MSInfo" into the start menu search bar. Select the "System Information" app that is suggested. A window shows up with a bunch of information about your computer
   2. In the search bar at the bottom of the "System Information" screen type "silabs" and hit enter.
   3. Validate that you see a record for a driver with Name "silabser" and Description "Silicon Labs CP210x USB to UART Bridge Driver"
