---
layout: page
title: Guide to Make for ARM cross-development
---

*Written for CS107E by Pat Hanrahan*

This guide is about using makefiles
for cross-development for the ARM.
We will briefly review makefile syntax and usage.
For more information about makefiles,
read the 
[CS107 Guide to Makefiles](http://web.stanford.edu/class/cs107/guide_make.html).

Below is a simple makefile that can be used
to build a binary for the ARM processor.

    #
    # Set the prefix used for ARM cross-development
    #
    ARMGNU = arm-none-eabi

    # The CFLAGS variable sets compile flags for gcc: 
    #
    #  -g        compile with debug information 
    #  -Wall     give verbose compiler warnings 
    #  -O2       generate level2 optimized code
    #
    # these options are required for bare metal code
    #
    #  -ffreestanding generate code assuming no operating system
    #  -nostartfiles do not call or link to startup code
    #  -nostdlib do not link against standard libraries
    #
    CFLAGS = -g -Wall -O2 -ffreestanding -nostdlib -nostartfiles

    on.bin: on.c
        $(ARMGNU)-gcc $(CFLAGS) -o on.o on.c
        $(ARMGNU)-objcopy on.o -O binary on.bin
        $(ARMGNU)-objdump on.o -d > on.list

