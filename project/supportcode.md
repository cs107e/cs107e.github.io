---
permalink: /project/supportcode
title: 'Available support code'
---

### Libmango reference modules
You are encouraged to use/adapt/learn from any of our published code as part of your project.
- Library of our reference modules is `$CS107E/lib/libmango.a`
    - Can also use your modules or your full library `libmymango.a`
- Source code for `libmango` support modules available in the `$CS107E/src` directory ([browse](/src))
    - `ccu` `de` `font` `hdmi` `hstimer` `gpio_interrupt` `interrupts ` `mango` `ps2_keys` `ringbuffer` `uart`

### Project code extras
The project [code_extras](code_extras) contains a hodgepodge of additional code resources. We have drivers written for some of common peripherals, various bits of sample code, and a few contributions shared by former students. You are welcome to incorporate any of this code into your project with proper citation. If you extend the code or add bug fixes, please contribute those modifications back to us for all to share!

- `spi` and `i2c`
    - these drivers have been successfully used in past, minor recent updates to code
- `pwm` and `i2s`
    - code for these modules is new-ish and needs further testing
- `math_lib`: student's implementation of math functions
- `neopixel`: sample code for Neopixel/WS8212 rgb led

[Browse code_extras directory](https://github.com/cs107e/cs107e.github.io/tree/master/project/code_extras)

We build a custom boot program ("secondary program loader" in linux-speak) to load/execute a MangoPi binary read from a microSD card (e.g. removes need to tether to your laptop for `xfel/mango-run` bootload). Successfully used Fall 2024.
- <https://github.com/cs107e/sdcard_boot_spl>

__Code sharing from previous projects__: A few of the projects built by our Mango pioneers (e.g. Daniel James' awesome 3-D graphics library) have been shared and extended in subsequent projects. We also have code in our archives on offer from previous student projects that implemented features on Raspberry Pi such as FAT filesystem, Arducam, GPU, ESP32 communication, etc. This older code could be interesting to learn from, but unlikely to be a simple recompile; effort to port could be anything from a minor nuisance and to significant rewrite.

Reach out if you want to know more and/or would like to offer your project code to future students.

### Code found out in the wild
Maker sites such as Adafruit, SparkFun, and DFRobot publish great tutorials and lots of example code. Their libraries tend to be well-tested and very complete, good resources to learn from!  Broad web search will unearth all kinds of random code on github and blogs; expect a hodge-podge in terms of quality and completeness. Note that code you find in the wild is very unlikely to be direct drop-in to our environment (RISC-V, Mango Pi, bare-metal), but studying the code can provide useful insights and a starting point for a port.


### Citation
If your project adopts from any resources you did not author yourself, they should be properly cited!
- Citation should include author, url, what was used of their work
- Explain what changes/extensions you contributed on top of the original (be specific)
- Your project will be primarily evaluated the portion of the work that you contributed

{% comment %}
- Code to read/write files on the [SD card](extras/sd_library)
- Using 2 ESP-32 devices to build a [Uart Wifi bridge](extras/uart-wifi-bridge) between 2 Raspberry Pis
- Using ESP-32 to [serve a web page](extras/webpage)
{% endcomment %}