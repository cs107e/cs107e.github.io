---
title: 'Guide: Installing the developer tools'
---

## Prerequisites
The installation guide assumes basic knowledge of the UNIX command line (opening a shell, entering commands, navigating file system, and editing text files). Those new to command line may want to read our [UNIX guide](/guides/unix) for a quick introduction and check out the [unix videos and reference material](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/unixref/)
prepared for CS107 students.

## Overview
You must configure your laptop with an environment suitable for developing programs to run on the Mango Pi. These development tools are unix-based. Mac OS and Linux natively support an appropriate environment, on Windows you will enable WSL (Windows Subsystem for Linux) and work in an Ubuntu instance running on top of Windows OS. If you're running Windows, make sure you've already done that with the [WSL setup guide](../wsl-setup).

<A name="installchecklist"></A>
The installation steps are:

- Install `riscv64-unknown-elf` cross-compile toolchain.
    A _cross-compiler_ is a development toolchain that runs on one system (e.g., x86) and generates machine code for a different system (in this case, RISC-V). The toolchain includes the C compiler and other essential development tools (assembler, linker, debugger, and utilities).
- Install `xfel`, our communication client for sending binaries to the Mango Pi (for Mac, this step is actually rolled into the toolchain installation, as you'll see in the steps).

Follow the installation instructions for your OS
+ [Install dev tools on macOS](../devtools-mac)
+ [Install dev tools on Windows WSL](../devtools-wsl)
+ [Install dev tools on ubuntu](../devtools-linux) (experimental)

<a name="finalcheck"></a>
## Final check steps
After completing the installation instructions, use these final check steps below to confirm your developer tools.

{% include checkstep.html content="confirm cross-compile debug with simulator" %}
```console?prompt=(gdb),$
$ riscv64-unknown-elf-gdb
GNU gdb (GDB) 13.2
... blah blah blah ...
(gdb) target sim
Connected to the simulator.
(gdb) quit
```

{% include checkstep.html content="confirm __xfel__  " %}
```console?prompt=$
$ xfel
xfel(v1.3.2) - https://github.com/xboot/xfel
usage:
    xfel version                                        - Show chip version
    xfel hexdump <address> <length>                     - Dumps memory region in hex
    xfel dump <address> <length>                        - Binary memory dump to stdout
    ... blah blah blah ...
```



