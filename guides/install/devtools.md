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
+ [Install on macOS](../devtools-mac)
+ [Install on Windows WSL](../devtools-wsl)

<a name="finalcheck"></a>
## Final check steps
After completing the installation instructions, use these final check steps below to confirm your developer tools.

{% include checkstep.html content="confirm $CS107E, cross-compile build" %}
```console
$ cd $CS107E/sample_build
$ make clean && make all
rm -f *.o *.bin *.elf *.list *~
riscv64-unknown-elf-gcc ... blah blah blah ...
```

{%- comment %}
NOTE: Removing GDB From instructions until we figure out how to use it in 13.2
```console?prompt=(gdb),$
$ riscv64-unknown-elf-gdb hello.elf
GNU gdb (GDB) 9.2
... blah blah blah ...
(gdb) target sim
Connected to the simulator.
(gdb) quit
```
{%- endcomment %}

{% include checkstep.html content="confirm __mango-run__ and __xfel__" %}
```console
$ mango-run
Usage: /Users/julie/cs107e_home/mycode/cs107e/bin/mango-run <binary-file>

  Simple script to run a program on the Mango Pi using xfel as bootloader.
  (xfel ddr d1, xfel write, xfel exec)

$ xfel version
ERROR: Can't found any FEL device.
```
> __got abort?__ If you run `xfel` when there are no USB devices connected to your laptop, rather than report a helpful error about not finding anything to talk to, `xfel` drops an unceremonious assert (see below). Re-connecting your Pi should fix it. If not, change to a different USB port on your laptop to jigger things, or if USB just seems wedged, restart your laptop.
{: .callout-danger}

```console
Assertion failed: (count > 0), function main, file main.c, line 60.
fish: Job 1, 'xfel version' terminated by signal SIGABRT (Abort)
```

