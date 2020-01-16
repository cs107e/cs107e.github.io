---
permalink: /resources/
title: Informational resources
toc: true
---

Here are links to various external resources with information about the Raspberry Pi, ARM assembly, C, and more.

### Raspberry Pi

* [Raspberry Pi Board Schematics](https://github.com/raspberrypi/documentation/blob/master/hardware/raspberrypi/schematics/README.md). In this course, we use the Raspberry Pi A+.

### ARM architecture

The Raspberry Pi A+ uses a Broadcom BCM2835 chip. 
Inside the chip is an ARM1176JFZ-S processor 
which is based on the ARMv6 architecture.

* The peripherals are documented in [Broadcom BCM2835 ARM Peripherals](/readings/BCM2835-ARM-Peripherals.pdf).

  * __Important:__ [Errata](http://elinux.org/BCM2835_datasheet_errata) for the above datasheet. Some of the issues are simple typos but there are places where the text is wrong or backwards, so keep the errata at hand to reconcile those.

* The ARM1176JFZ-S processor is documented in the 
[Technical Reference Manual](/readings/arm1176.pdf).  

* ARM1176JFZ-S processors use the ARMv6 architecture,
which is documented in the [ARMv6 Architecture Manual](/readings/armv6.pdf). At 1000 pages, the full manual is quite a tome; here is an [excerpt for the instruction set](/readings/armisa.pdf).

* ARM System-on-Chip Architecture (2nd Edition), Steve Furber. <https://www.amazon.com/dp/0201675196/>
Steve Furber was the principal design of the first ARM processor.
The diagrams of the ARM architecture are based on the diagrams
in Chapter 4: ARM Organization and Implementation.

* ARM System Developer's Guide: Designing and Optimizing System Software, Andrew Sloss, Dominic Symes, and Chris Wright, 2004. <https://www.amazon.com/dp/1558608745/> This book is a bit dated, but still provides an excellent overview of low-level ARM programming. 

* Excellent [slides](http://twins.ee.nctu.edu.tw/courses/ip_core_02/handout_pdf/Chapter_2.pdf) by Prof. Yen at NCTU on the ARM processor core and instruction sets.

### ARM Assembly Language

* Carl Burch's
  [Introduction to ARM Assembly Language](http://www.toves.org/books/arm/)
  is a highly recommended starting point.

  *Note: Burch's document uses the
  [older ARM syntax](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0773a/chr1383143764305.html)
  instead of the GNU syntax we use.*

* A [Whirlwind Tour of ARM Assembly](http://www.coranac.com/tonc/text/asm.htm) from the TONC Guide to programming the Nintendo Game Boy Advance (which uses am ARM processor).

* Davespace's [Introduction to ARM Course](http://www.davespace.co.uk/arm/introduction-to-arm/index.html).

* Think in Geek article series [ARM Assembler for the Raspberry Pi](http://thinkingeek.com/2013/01/09/arm-assembler-raspberry-pi-chapter-1/).

### Bare Metal Programming

* David Welch's extensive github repository of [bare metal programming examples](https://github.com/dwelch67/raspberrypi).

* Steve Halliday's [videos](http://computersciencevideos.org/Raspberry-Pi/Raspberry-Pi-Setup) about programming the raspberry pi.

* Alex Chadwick's [Baking Pi Course](http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/).

* The [Raspberry Pi Bare Metal Forum](http://www.raspberrypi.org/forums/viewforum.php?f=72) has lots of information from experienced developers. Venturing into the forum is a great way to learn advanced techniques. 

### C Programming Language

* Nick Parlante's [EssentialC](http://cslibrary.stanford.edu/101)

* *The C Programming Language, 2nd Ed.*, B. Kernighan and D. Ritchie.
A digital copy of K&R is available to Stanford students via 
[Safari Books Online](https://proquest-safaribooksonline-com.stanford.idm.oclc.org/9780133086249)

* *The Definitive Guide to GCC, 2nd Ed.*, William von Hagen, Apress, 2006
[pdf](http://sensperiodit.files.wordpress.com/2011/04/hagen-the-definitive-guide-to-gcc-2e-apress-2006.pdf)
