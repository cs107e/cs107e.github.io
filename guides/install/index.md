---
title: Installation Guide
---

### Prerequisites
The installation guide assumes basic knowledge of the UNIX command line (opening a shell, entering commands, navigating file system, editing text files). Read our [UNIX guide](/guides/unix) for a quick introduction.
Those new to UNIX or wishing to brush up their skills may want to check out the
library of [introductory unix videos/guides](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/unixref/)
maintained on the standard CS107 web site.

### Install steps
In this course, you will use your laptop as a development environment to write programs that run on the Raspberry Pi. There are several steps you must take to configure your laptop with the needed tools and environment.

1. _WINDOWS-ONLY_ Follow our [Windows VM install instructions](vm) to first install a virtual machine running Linux.
1. Follow [these common instructions](common) to:
    + Create a `cs107e_home` directory to store your class materials. 
    + Configure the `git` version control system. We distribute course materials as git repos and you will use git to access, manage, and submit your work. 
1. _MAC-ONLY_ Follow [Mac install instructions](mac) to:
    + Install the `arm-none-eabi` cross-compile toolchain.  A _cross-compiler_ is a development toolchain that runs on one system (e.g., x86) and generates machine code for a different system (in this case, ARM). The toolchain includes the C compiler and other essential development tools (assembler, linker, debugger, and utilities).  
    + Install python3, package prerequisites, and console driver needed by the bootloader client `rpi-install.py`.

You do not need step 1 on Mac OS because the tools are installed directory without a VM. 
You do not need step 3 on the VM because the VM has the toolchain and python3 pre-installed.

### Install checklist
After completing the install steps, use the following checklist to verify your environment configuration.

#### cs107e home
✔️__Check:__ confirm environment variables CS107E and PATH are properly set with the following commands:

```
$ ls $CS107E/lib/libpi.a
/Users/myname/cs107e_home/cs107e.github.io/cs107e/lib/libpi.a

$ pinout
O-------------------------------O
| oooooooooooooooooooo J8       |
| 1ooooooooooooooooooo          |
...
```

#### git
✔️__Check:__ confirm your git configuration with the following command:

```
$ git config --get-regexp user
user.name My Full Name
user.email myemail@stanford.edu
```


#### arm-none-eabi toolchain
✔️__Check:__ confirm arm-none-eabi toolchain with the following command:
```
$ arm-none-eabi-as --version
GNU assembler (GNU Binutils) 2.24
Copyright 2013 Free Software Foundation, Inc.
```

#### rpi-install.py
✔️__Check:__ confirm rpi-install.py with the following command:

```
$ rpi-install.py -h
usage: rpi-install.py [-h] [-v] [-q] [-t T] [-T T] [-p | -s] [port] file

This script sends a binary file to the Raspberry Pi bootloader. Version 1.0.
```


