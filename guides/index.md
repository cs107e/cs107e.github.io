---
title: Course Guides
---

We prepared these guides on various topics relevant to CS107E students. Hope you find them useful.

## Review

- [Electricity](electricity) basics
- [Binary and hexadecimal](numbers) number representations
- Intro to [bitmasking](bitmasking)
- Using the [unix command line](unix)

## Kit and Tools
- [Bill of Materials (BOM)](bom) for course parts kit
- Recommendations for [hand tools](handtools)
- [Soldering guide](soldering-guide.pdf) for soldering pins onto the MangoPi 

## Installation
- [Install WSL](install/wsl-setup) (Windows users only)
- [Setup of git and mycode repo](setup-mycode-repo)
- [Install developer tools](install/devtools) (riscv toolchain and xfel)
- [Install CP2102 driver](install/cp2102) and [Minicom](install/minicom) for USB-serial device

## Mango Pi
- [Powering the Mango Pi](power)
- [Using `xfel` as a bootloader](xfel)
- [Reference card with pinout](refcard) (printed copy in your kit)
- [Serial communication over uart](uart)
- [Meet your GPIO pins](MeetYourGPIOPins.pdf)
- [Program your GPIO pins](ProgramYourGPIOPins.pdf)


## Risc-V
- [One page of RISC-V](riscv-onepage)

## Development tools/process
- The [git workflow used for labs and assignments](cs107e-git) in CS107e
- [Using `git` for version control](git)
- [Bare metal programming using `gcc`](gcc)
- [`make` and `Makefiles`](make)
{%- comment %}- [Using `gdb` in simulation mode](gdb){% endcomment %}
- [Binary utilities (`binutils`)](binutils)
- [Advice on software testing](testing)

## Extras
These resources are not fully fleshed out, but could be helpful starting points to use for projects
- Info on the [vector unit](extras/vector_unit) of Mango Pi
- Sensors lecture [code examples](https://github.com/cs107e/cs107e.github.io/tree/master/lectures/Sensors/code) (includes bit bang version of i2c and spi)
- Code to use [spi peripheral](https://github.com/cs107e/cs107e.github.io/tree/master/cs107e/extras/spi_peripheral) of Mango Pi
- Implementation of [math library](https://github.com/cs107e/cs107e.github.io/tree/master/cs107e/extras/mathlib/) of Mango Pi
- Sample code for [Neopixel/WS8212](https://github.com/cs107e/cs107e.github.io/tree/master/cs107e/extras/neopixel/)

{% comment %}
- Code to read/write files on the [SD card](extras/sd_library)
- Using 2 ESP-32 devices to build a [Uart Wifi bridge](extras/uart-wifi-bridge) between 2 Raspberry Pis
- Using ESP-32 to [serve a web page](extras/webpage)
{% endcomment %}
