---
title: 'Guide: Installing the developer tools'
---

## Prerequisites
The installation guide assumes basic knowledge of the UNIX command line (opening a shell, entering commands, navigating file system, and editing text files). Those new to command line may want to read our [UNIX guide](/guides/unix) for a quick introduction and check out the [unix videos and reference material](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/unixref/)
prepared for CS107 students.

## Overview
You must configure your laptop with an environment suitable for developing programs to run on the Raspberry Pi. These development tools are unix-based. Mac OS and Linux natively support an appropriate environment, on Windows you will enable WSL (Windows Subsystem for Linux) and work in an Ubuntu instance running on top of Windows OS.

<A name="installchecklist"></A>
The installation steps are:

- Install `arm-none-eabi` cross-compile toolchain.
    A _cross-compiler_ is a development toolchain that runs on one system (e.g., x86) and generates machine code for a different system (in this case, ARM). The toolchain includes the C compiler and other essential development tools (assembler, linker, debugger, and utilities).  
- Install python3, package prerequisites, and CP2102 console driver needed by the bootloader client `rpi-run.py`.
- Create a `cs107e_home` directory to store your class materials
- Configure shell and git environment

Follow the installation instructions for your OS
+ [Install on macOS](../install/mac)
+ [Install on Windows WSL](../install/wsl)

## Final check steps
After completing the installation instructions, use these final check steps below to confirm your environment from top to bottom.

{% include checkstep.html content="confirm $CS107E, cross-compile build and debugger __with simulator__" %}
```console?prompt=(gdb),$
$ cd $CS107E/sample_build
$ make clean && make all
rm -f *.o *.bin *.elf *.list *~
arm-none-eabi-gcc ... blah blah blah ...
$ arm-none-eabi-gdb hello.elf
GNU gdb (GDB) 9.2
... blah blah blah ...
(gdb) target sim
Connected to the simulator.
(gdb) quit
```

{% include checkstep.html content="confirm __rpi-run.py__ version 2.3" %}
```console
$ rpi-run.py -h
usage: rpi-run.py [-h] [-d device] [-v]  [-p | -s] [file]

This script sends a binary file to the Raspberry Pi bootloader. Version 2.3
```

{% include checkstep.html content="confirm __CP2102 driver__" %}
If you have your [CP2012 USB-serial breakout board](/guides/bom), plug it into a USB port on your computer and confirm it can be found. 
```console
$ rpi-run.py
Found serial device: /dev/ttyS3
```
The reported device name can vary, e.g. `/dev/cu.usbserial` or `/dev/cu.SLAB_USBtoUART` and others.
