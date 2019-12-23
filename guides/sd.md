---
title: Working with SD cards
toc: true
---

*Written by Pat Hanrahan*

Your Raspberry Pi kit contains a microSDHC card. A secure digital (SD) card
stores data in non-volatile memory. The HC in SDHC stands for high
capacity. A micro-SD is shown below on the right. The adapter "jacket" on the left allows the small card to be used in a full-size SD card reader.

![micro card with adapter](../images/sd.kingston.jpg)

When a Raspberry Pi boots, it accesses the card to read the file named `kernel.img` and executes that program.

1. Prepare the card with the proper files by mounting it on your laptop and copying the necessary files onto it. 
2. Eject the card from your laptop and insert it into Raspberry Pi.  
3. When the Pi boots, it runs the program on the card.
4. To change the program, repeat the process and copy the updated file. A much less tedious long-term solution is running a [bootloader](/guides/bootloader).

#### Mount SD card on laptop
To copy a program to the SD card, you need to mount it on your laptop. If your laptop does not have a built-in SD card reader, you will need an external reader (we have a few to loan in lab). To mount the card, first insert the micro-SD into the adapter, and then insert the adapter into the SD card slot.

![insert sd card into slot](../images/sd.mac.jpg)

The SD card mounts automatically and will show up in your finder along with other mounted file systems.

Verify that the card is mounted.

    $ ls /Volumes
    Macintosh HD    NO NAME

By default, the volume is named `NO NAME`.


#### Copy files onto card

The necessary files are stored in the firmware directory of our [courseware
repository](https://github.com/cs107e/cs107e.github.io/tree/master/firmware). There are 4 files.

    $ ls firmware
    blink-actled.bin   bootcode.bin     bootloader.bin  start.elf     

`bootcode.bin` and `start.elf` are the start files necessary for the GPU. The Pi boot sequence next looks for a file called `kernel.img`, the program to run. Normally `kernel.img` is the linux kernel. In this course, we will replace the linux kernel with our
own program.

Our firmware folder has two programs, `blink-actled.bin` and
`bootloader.bin`.  As a first step, copy `blink-actled.bin`
to `kernel.img`. This program
blinks the on-board activity (ACT) LED. This is a
good way to test whether your Pi is working.

Later, after you've verified the blink program, you can
replace `kernel.img` with `bootloader.bin` so that Pi will boot and run a boot
loader that can receive uploaded programs. 

Copy the firmware files onto your SD card and eject it. On a Mac, click on the Eject icon next to the volume name in the Finder.

#### Insert card into Raspberry Pi
Remove the micro-SD from its adapter and insert it into the slot on the underside of the Raspberry Pi circuit board. There is a small spring that holds the card in place. As you push it in, you will feel the mechanism grab onto the card.

![micro SD inserted on Pi](../images/sd.pi.jpg)

{% include callout.html type="danger" %}
**Take care!** To eject the micro-SD from the Pi's card slot, gently push the card in and allow it to spring back out. If you try to pull out the card by force, you can break the mechanism and potentially destroy your Pi.
</div>

### Troubleshooting

1.  The SD cards we are using are formatted with a FAT32 filesystem. If this
    file system is corrupted, you will need to reformat the file system.
    Consult the internet for how to do this.
2.  The Raspberry Pi can become wedged if `kernel.img` has bugs or
    becomes corrupted. You can always recover to a known state by copying the
    original firmware to the SD card.

