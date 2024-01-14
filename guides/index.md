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

## Installation
- [Install WSL](install/wsl-setup) (Windows users only)
- [Setup of git and mycode repo](setup-mycode-repo)
- [Install developer tools](install/devtools) (riscv toolchain and xfel)
{% comment %}
- [Install CP2102 driver and minicom](install/serial.md) for serial communication over uart
{% endcomment %}

## Mango Pi
- [Powering the Mango Pi](power)
- [Using `xfel` as a bootloader](xfel)
- [Reference card with pinout](refcard) (printed copy in your kit)
- [Serial communication over uart](uart)

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

{% comment %}
## Extras
These resources have not been thoroughly vetted, but were successfully used Fall 2019.

- Code to read/write files on the [SD card](extras/sd_library)
- Using 2 ESP-32 devices to build a [Uart Wifi bridge](extras/uart-wifi-bridge) between 2 Raspberry Pis
- Using ESP-32 to [serve a web page](extras/webpage)
{% endcomment %}
