---
title: Installation Guide
---

### Prerequisites
The installation guide assumes basic knowledge of the UNIX command line (opening a shell, entering commands, navigating file system, editing text files). Read our [UNIX guide](/guides/unix) for a quick introduction.
Those new to UNIX may want to check out the [unix videos/guides](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/unixref/)
prepared for CS107 students.

### ✔️Checkpoints
We peppered our instructions with ✔️__Check__ points that confirm your progress through the steps. Use them to validate a task before moving on to the next step. Do not be alarmed about the long-winded unix-babble produced during installation -- these tools are total chatterboxes. If you hit a snag, ask on Piazza or come to office hours and we can help you out!

### Overview
You must configure your laptop with a development environment capable of writing and compiling programs to run on the Raspberry Pi. The development tools are unix-based. Mac OS and Linux natively support an appropriate environment, on Windows you must enable WSL (Windows Subsystem for Linux) and work in an Ubuntu instance running on top of Windows OS.

The installation steps are:

<A name="steps"></A>
1. Install developer tools:
    + `arm-none-eabi` cross-compile toolchain.  A _cross-compiler_ is a development toolchain that runs on one system (e.g., x86) and generates machine code for a different system (in this case, ARM). The toolchain includes the C compiler and other essential development tools (assembler, linker, debugger, and utilities).  
    + python3, package prerequisites, and console driver needed by the bootloader client `rpi-install.py`.
    + Choose the appropriate instructions for your OS
        +  [Install dev tools on MacOs](../install/mac)
        +  [Install dev tools on Windows](../install/wsl)
1. Configure user account:
    + create a `cs107e_home` directory to store your class materials
    + configure shell and git environment
    + Follow same instructions for all
        + [Configure home/user](../install/userconfig)

### Final checklist
After completing above steps, use the list below to confirm your environment from top to bottom.

#### arm-none-eabi toolchain
✔️__Check:__ confirm toolchain (ok if version is newer):
```
$ arm-none-eabi-as --version
GNU assembler (GNU Binutils) 2.24
Copyright 2013 Free Software Foundation, Inc.
```
#### arm-none-eabi-gdb with simulator
✔️__Check:__ start debugger and confirm ARM simulator is available:
```
$ arm-none-eabi-gdb
GNU gdb (GDB) 7.8.1
Copyright (C) 2014 Free Software Foundation, Inc.
 .... blah blah blah ...
(gdb) target sim
Connected to the simulator.
(gdb) quit
```


#### rpi-install.py
✔️__Check:__ confirm rpi-install.py version 1.1:

```
$ rpi-install.py -h
usage: rpi-install.py [-h] [-d device] [-v]  [-p | -s] [file]

This script sends a binary file to the Raspberry Pi bootloader. Version 1.1.
```

#### cs107e home
✔️__Check:__ confirm environment variables CS107E and PATH:

```
$ ls $CS107E/lib/libpi.a
/Users/myname/cs107e_home/cs107e.github.io/cs107e/lib/libpi.a

$ pinout
O-------------------------------O
| oooooooooooooooooooo J8       |
| 1ooooooooooooooooooo          |
|                               |
|[RUN]   Pi Model A+ V1.1   +=====
...
```

#### git
✔️__Check:__ confirm your git identity:

```
$ git config --get-regexp user
user.name My Full Name
user.email myemail@stanford.edu
```
