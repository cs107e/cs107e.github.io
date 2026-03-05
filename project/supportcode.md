---
permalink: /project/supportcode
title: 'Support code'
---

### Libmango reference modules
You are encouraged to use/adapt/learn from any of our published code as part of your project.
- Library of our reference modules is `$CS107E/lib/libmango.a`
    - Can also use your modules or your full library `libmymango.a`
- Source code for `libmango` support modules available in the `$CS107E/src` directory ([browse](/src))
    - `ccu` `de` `font` `hdmi` `hstimer` `gpio_interrupt` `interrupts ` `mango` `ps2_keys` `ringbuffer` `uart`

### Libextra
Libextra adds modules for D1-H hardware peripherals that may be of interest for projects.
- `dma` `spi` `i2c` `i2s`
    - robust and comprehensive, well-tested
    - These drivers are the work of the awesome Curis 2025 team. Thank you, Daniel, Eli, and Joe!
- `pwm`
    - draft version, limited functionality, thinly tested
- Source code for `libextra` modules available in the `$CS107E/src` directory ([browse](/extra))
    - `dma` `spi` `i2c` `i2s` `pwm`

### Boot disk
We built a custom boot program ("secondary program loader" in linux-speak) that can load and execute a program binary read from a microSD card. This removes need to tether to your laptop for `xfel/mango-run` bootload. Check out this repo to learn more: <https://github.com/cs107e/sdcard_boot_spl>

### Code gallery
We have a smattering of demo code and contributions shared by former students. You are welcome to incorporate any of this code into your project with proper citation. If you extend the code or add bug fixes, please contribute those modifications back to us for all to share! [Browse code gallery](https://github.com/cs107e/cs107e.github.io/tree/master/project/code_gallery). And let us know if you have a contribution to the gallery for benefit of future stuents.

- `math_lib`: Calvin's implementation of math functions
- `neopixel`: sample code for Neopixel/WS8212 rgb led
- `3dgraphics`: 3D graphics library from Daniel and Ege


__Code sharing from previous projects__: Some of the projects by our Mango pioneers (e.g. 3-D graphics library) have been shared and extended in subsequent projects. We also have code in our archives from previous student projects on Raspberry Pi for a FAT filesystem, Arducam, ESP32 communication, etc. This older code could be interesting to learn from, but unlikely to be a simple recompile; effort to port could be anything from a minor nuisance and to significant rewrite. Reach out if you want to know more about this code.

### Code found out in the wild
Maker sites such as Adafruit, SparkFun, and DFRobot publish great tutorials and lots of example code. Their libraries tend to be well-tested and very complete, good resources to learn from!  Broad web search will unearth all kinds of random code on github and blogs; expect a hodge-podge in terms of quality and completeness. Note that code you find in the wild is very unlikely to be direct drop-in to our environment (RISC-V, Mango Pi, bare-metal), but studying the code can provide useful insights and a starting point for a port.

### Citation
If your project adopts from any resources you did not author yourself, they should be properly cited!
- Citation should include author, url, what was used of their work
- Explain what changes/extensions you contributed on top of the original (be specific)
- Your project will be primarily evaluated the portion of the work that you contributed
