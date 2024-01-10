---
title: 'Guide: Installing CP2102 driver and minicom'
---

TODO: CP2102 + minicom

{% include checkstep.html content="confirm __CP2102 driver__" %}
If you have your [CP2012 USB-serial breakout board](/guides/bom), plug it into a USB port on your computer and confirm it can be found.
```console
$ rpi-run.py
Found serial device: /dev/ttyS3
```
The reported device name can vary, e.g. `/dev/cu.usbserial` or `/dev/cu.SLAB_USBtoUART` and others.
