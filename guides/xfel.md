---
title: "Guide: Using xfel as a bootloader"
toc: true
attribution: Written by Julie Zelenski
---

## What is a bootloader?
You will edit and compile programs on your laptop, and you will need to transfer the compiled program to the Pi when ready to execute it. One way to move a program from your laptop to the Pi is using a micro-SD card. You would prepare a card with the early-stage boot code and add your program to be launched afer booting.  Each time you update the program, you would re-insert the card on your laptop,
copy the latest program onto it, eject the
card and then insert into Pi. You can see how this process would quickly become tedious!

Instead, you can set up a connection between your laptop and the Pi and use a __bootloader__ to transfer the program. The bootloader runs on the Pi and waits, listening for a request. When you send a program from your laptop,
the waiting bootloader receives it and writes the program to the
memory of the Pi, a process called "loading" the program. To execute the program, you tell the bootloader to jump to the start address of the program.

Using a bootloader, the cycle to run a newly-compiled program is:

1.  Reset the Pi to re-enter the bootloader
2.  Use `xfel` on your laptop to send the program to the bootloader and execute it on the Pi

The bootloader we are using is called `FEL/xfel`. `FEL` (firmware exchange launch) is a low-level subroutine contained in the BootROM on Allwinner devices. It is used for initial programming and recovery of devices over USB.

BootROM is a small piece of read-only/write-protected memory embedded in the processor. It contains the very first code which is executed by the processor on power-on or reset. When the Pi is powered on, it enters FEL by default.  FEL does a minimal first-stage boot and waits for further commands to come over the USB port.

The `xfel` tool runs on your laptop and communicates with the bootloader.

> __Got xfel?__ You should have installed `xfel` as part of installing the developer tools.  If you skipped this step, do it now. See section __xfel__ in the [installation guide](/guides/install/devtools) for your OS.
{: .callout-warning}

## Connect through OTG
FEL listens for commands over USB. The connection from your laptop to the OTG port that is [powering the Pi](/guides/power) is the same connection your laptop will use to communicate with the bootloader.  Be sure that your usb cable/hub supports both charge and data, needed for power and communication, respectively.

![usb otg power](../images/power-otg.png)
{: .w-25 .mx-auto }


Invoking `xfel` with no arguments shows the available commands. The commands `xfel ddr`, `xfel write` and `xfel exec` are the ones we will use most.

```console?prompt=$
$ xfel
xfel(v1.3.2) - https://github.com/xboot/xfel
usage:
    xfel version                                        - Show chip version
    xfel hexdump <address> <length>                     - Dumps memory region in hex
    xfel dump <address> <length>                        - Binary memory dump to stdout
    xfel read32 <address>                               - Read 32-bits value from device memory
    xfel write32 <address> <value>                      - Write 32-bits value to device memory
    xfel read <address> <length> <file>                 - Read memory to file
    xfel write <address> <file>                         - Write file to memory
    xfel exec <address>                                 - Call function address
    xfel reset                                          - Reset device using watchdog
    xfel sid                                            - Show sid information
    xfel jtag                                           - Enable jtag debug
    xfel ddr [type]                                     - Initial ddr controller with optional type
    xfel sign <public-key> <private-key> <file>         - Generate ecdsa256 signature file for sha256 of sid
    xfel spinor                                         - Detect spi nor flash
    xfel spinor erase <address> <length>                - Erase spi nor flash
    xfel spinor read <address> <length> <file>          - Read spi nor flash to file
    xfel spinor write <address> <file>                  - Write file to spi nor flash
    xfel spinand                                        - Detect spi nand flash
    xfel spinand erase <address> <length>               - Erase spi nand flash
    xfel spinand read <address> <length> <file>         - Read spi nand flash to file
    xfel spinand write <address> <file>                 - Write file to spi nand flash
    xfel spinand splwrite <split-size> <address> <file> - Write file to spi nand flash with split support
    xfel extra [...]                                    - The extra commands
```
Try `xfel version` now. If you have a valid connection to the Pi and the bootloader is running, it should respond with the chip id.
```console
$ xfel version
AWUSBFEX ID=0x00185900(D1/F133) dflag=0x44 dlength=0x08 scratchpad=0x00045000
```

If the Pi is not powered on or is not connected, `xfel` cannot find the device and will report this error.

```console
$ xfel version
ERROR: Can't found any FEL device
```

If the Pi is powered on and connected but the bootloader is not running, `xfel` will find the device and attempt to communicate with it, but since the bootloader is not listening, the message will fall on deaf ears. After a few unacknowledged retries, it will give up and report this error:

```console
$ xfel version
usb bulk send error
```

## Use xfel read32/write32 to peek and poke
The commands `xfel read32` and `xfel write32` can be used to peek and poke values in memory. Admittedly these commands are more of a cute party trick than a useful way to control the Pi.

Try out these commands on the address of a GPIO peripheral register.
The blue on-board act led is controlled by GPIO PD18. The base address of config0 register for the PD group is `0x02000090`. The function of PD18 is set in config2 at address `0x02000098`. The data register for the PD group is address `0x020000a0`, the value for PD18 corresponds to the 18th bit in the register.  The exchange below shows reading the value of config2 and data and then writing to config2 and data to configure PD18 as an output and set its value to 1. The blue act led on-board the Mango Pi should now turn on.

```console
$ xfel read32 0x02000098
0x0fffffff
$ xfel read32 0x020000a0
0x00000000
$ xfel write32 0x02000098 0x100
$ xfel write32 0x020000a0 0x40000
```

The command `xfel hexdump` will read a range of memory and printf it.  Below shows a hexdump of some of the memory used for the GPIO peripheral registers.
```console
$ xfel hexdump 0x02000000 100
02000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
02000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
02000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
02000030: ff ff ff ff 66 ff 0f 00 00 00 00 00 00 00 00 00  ....f...........
02000040: 00 00 00 00 11 11 11 11 11 11 01 00 00 00 00 00  ................
02000050: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
02000060: ff ff ff ff                                      ....
```

## Use xfel to load and execute a program
Before loading a program, we must first bring up the memory controller. This requires a finely-tuned bit of hardware wizardry that is supplied by `xfel`. We will use the command `xfel ddr d1` to properly initialize the memory for the D1. You must always initialize the memory controller after reset and before running a program.

Once the memory system is operationg, use the command `xfel write` to send a binary file from your laptop to the bootloader. The bootloader will receive the file and writes it to memory at the address you specify. The command `xfel exec` will start execution at that address. We load our programs to address `0x40000000`, the standard start location for RISC-V.

Try these commands out now. Change to the CS107E bin directory and send the blink-actled program:
```console
$ cd $CS107E/bin
$ xfel ddr d1
Initial ddr controller succeeded
$ xfel write 0x40000000 blink-actled.bin
100% [================================================] 44.000 B, 23.620 KB/s
$ xfel exec  0x40000000
```
The blink-actled program sits in an endless loop blinking the blue on-board led.

To load and run a different program, your must first reset the Pi by briefly interrupting power. This step is necessary to reset the Pi and reenter the bootloader so it is ready to receive a new program.

## Pro-tip: `mango-run`
You will need to re-play these commands (`xfel ddr d1; xfel write; xfel exec`) so often that we wrote a small script to package this command sequence into the single command `mango-run`.

If you invoke `mango-run` with no arguments, it does a version check to confirm the Pi is connected and bootloader running and listening.

```console
$ mango-run
Usage: ./mango-run <binary-file>

  Simple script to run a program on the Mango Pi using xfel as bootloader.
  (xfel ddr d1, xfel write, xfel exec)

  xfel version
  AWUSBFEX ID=0x00185900(D1/F133) dflag=0x44 dlength=0x08 scratchpad=0x00045000
```

If you invoke `mango-run` with name of a binary program, it will send that to the bootloader and execute it:

  ```console
$ cd $CS107E/bin
$ mango-run blink-actled.bin
xfel ddr d1
Initial ddr controller succeeded
xfel write 0x40000000 blink-actled.bin
100% [================================================] 44.000 B, 20.298 KB/s
xfel exec 0x40000000
```
There you have it, a simple way to execute programs on your Pi. Nifty!

<a name="troubleshooting"></a>
## Troubleshooting 
Some suggestions of how to diagnose and resolve troubles with the bootloader.

Use `xfel version` (or `mango-run` with no arguments) to test communication with the bootloader.
- If response is chip id `AWUXBFEX ID=...`, you are good to go! It found the connection to the pi, the bootloader is running, and the Pi responded with the id. All's right in your world.
- If response is `ERROR: Can't found any FEL device`, it could not find the Pi to connect.
	- Double-check the Pi is powered up, that your laptop is connected to the Mango OTG port, and the cable you are using supports both charge and data.
	- In some situations, a cable/connection that were working perfectly fine a moment ago can suddenly stop working. This can happen if your laptop's usb system went a bit wonky and suspended the port. Try unplug from your laptop usb port and replug to prompt your laptop to resume. In some cases, you may need to reboot your laptop to set things right.
- If response is `usb bulk send error`, it found the connection to Pi, but was not able to communicate with the bootloader.
	- This generally means the bootloader is not running/listening. The most common cause would be that you have already bootloaded a program and it is executing, thus the bootloader is no longer running. To re-enter the bootloader, you must restart the Pi. Reset by briefly cutting power to it by unplug/replug or use the switch on your hub/cable.

## Resources
- The `xfel` source code is published on github <https://github.com/xboot/xfel>
- Documentation on `xfel` is available at <https://xboot.org/xfel/>
- The `mango-run` script is available in `$CS107E/bin`.
