---
title: Using WSL in CS107E
toc: true
---

These are the instructions for setting up your development environment in Windows using the Windows Sub-System for Linux (WSL).

You may need to restart your computer a few times throughout this process, so it’s a good idea to save all your work before starting. It’s easiest to restart your computer when prompted, so we recommend just restarting when prompted.

### Requirements

- Windows 10: These instructions were tested on Windows 10. If you're using Windows 8, let one of the instructors know so we can help get you set up

### Enable and Install WSL with Ubuntu 16.04

#### Enable WSL

We'll first need to enable WSL in Windows 10.

1. Open the settings app and navigate to Apps -> Apps & Features
2. Scroll to the bottom of the page, and click the link that says "Programs and Features". A new window will open with a list of applications installed on your machine
3. On the left side of the screen, click the link titled "Turn Windows features on or off". Another window should open with a list of checkboxes and titles
4. Find the checkbox for "Windows Subsystem for Linux". Check it, and click "OK". An "Applying changes window will open. Wait for it to finish. Once it's finished, restart your computer.

#### Install Ubuntu 16.04

We'll be using the Ubuntu 16.04 installation on windows for the course.

1. Open the windows store app and type "Ubuntu" in the search bar.
2. Select the result titled "Ubuntu 16.04 LTS" and click Install on the app landing page
3. Once it's done installing, launch the application. A terminal window will open that says "Installing, this may take a few minutes...".
4. Once it's done installing, you'll be prompted to enter a new username and Password. This will be your username and password for your Ubuntu WSL installation. You’ll need to use these when you access administrator privileges, so make sure you remember it!
5. Once you're done creating your user account and password, you'll have a normal linux terminal to work in! It's always a good idea to update new systems, so we'll do that first. Run the following commands:

   ```
   $ sudo apt update
   $ sudo apt upgrade
   ```

Now we have an up-to-date version of Ubuntu running on our Windows machine!

### Install Python3 and packages

The `rpi-install.py` script will be used to send programs from your computer to the Pi. This script requires python3 and two support modules to be installed on your laptop.

Note: Ubuntu 16.04 comes with a version of python installed, but it is an older version (version 2) which is nearing end of life. You need version 3. If you previously have installed python3, skip step 1 below.

1.  Install python3. Run the following command from your WSL terminal:

    ```
    $ sudo apt install python3
    ```

    ✔️**Check:** use the following command to confirm `python3` installation:

    ```
    $ python3 --version
    Python 3.8.1
    ```

2.  Install pip for python3

    ```
    $ sudo apt-get install python3-pip
    ```

    ✔️**Check:** use the following command to confirm `pip3` installation:

    ```
    $ which pip
    ```

3.  Use the following command to install the pyserial and xmodem packages for python3:

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

### Install arm-none-eabi toolchain

This is the toolchain we need in order to compile code for the Raspberry Pi.

1. Install the gcc compiler

   ```
   $ sudo apt install gcc-arm-none-eabi
   ```

2. Install the gdb debugger
   ```
   $ sudo apt install gdb-arm-none-eabi
   ```

### Install CP2012 console driver

The console driver enables the bootloader client to communicate with the Pi over the USB-serial device.

1. Download the CP2012 driver. The drivers are available on the Silicon Labs [CP210x Downloads page](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers). Scroll down to the section titled "Download for Windows 10 Universal" and download the VCP zip file. Decompress the zip file and open the containing folder. You should see a few folder s and files. Look for the file named "CP210xVCPInstaller_x64.exe". Double click this file to run the executable and follow the prompts on the screen.

(If you have a 32 bit machine, you should run the file named "CP210xVCPInstaller_x86.exe instead. To check which version of windows your running, follow the instructions [here](https://support.microsoft.com/en-us/help/13443/windows-which-version-am-i-running))
