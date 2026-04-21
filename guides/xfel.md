---
title: "Guide: Using xfel as a bootloader"
toc: true
attribution: Written by Julie Zelenski
---

## What is a bootloader?
You will use your laptop to edit and compile programs, and transfer the compiled binary to the Pi to execute it. One way to transfer a program from your laptop to the Pi would be to use a micro-SD card. You would prepare a card with the first-stage boot code and add your program to be run afer booting. Each time you updated your program, you would need to re-insert the card on your laptop,
copy the latest program onto it, eject the
card and then insert into Pi. You can see how this process would quickly become tedious!

Instead, we can make a wired connection between your laptop and the Pi and use a __bootloader__ to transfer the program. The bootloader program runs on the Pi at startup, and sits in a loop, waiting for a request. You initiate contact with the bootloader and transfer the compiled program.  The bootloader receives the program and writes it to the Pi's memory, a process called "loading" the program. To execute the program, you tell the bootloader to jump to the start address of the program.

We use the `FEL` bootloader provided by the D1-H SOC.  `FEL` (firmware exchange launch) is a low-level subroutine contained in the BootROM of Allwinner devices. It is used for initial programming and recovery of devices over USB.

BootROM is a small piece of read-only/write-protected memory embedded in the processor. It contains the very first code which is executed by the processor on power-on or reset. When the Pi is powered on, it enters FEL by default.  FEL does a minimal first-stage boot and waits for further commands to come over the USB port.

The `xfel` tool runs on your laptop and communicates with the bootloader over the USB port.

Using a bootloader, the cycle to run a program on the Pi is:

1.  Reset the Pi to re-enter the bootloader
2.  Use `xfel` on your laptop to send the program to the bootloader and execute it


> __Got xfel?__ You should have installed `xfel` as part of installing the developer tools. If you missed this step, do it now. See section __xfel__ in the [installation guide](/guides/install/devtools) for your OS.
{: .callout-warning}

## Connect through OTG
FEL listens for commands over USB. The connection from your laptop to the OTG port that is [powering the Pi](/guides/power) is the same connection your laptop will use to communicate with the bootloader. Your USB cable/hub must support both charge and data, needed for power and communication, respectively.

![usb otg power](../images/power-otg.jpg)
{: .w-50 .mx-auto }


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

If the Pi is not powered on or is not connected, `xfel` will not be able to find the device and will report this error.

```console
$ xfel version
ERROR: xfel found no connected FEL device.
```

If the Pi is powered on and connected but the bootloader is not running, `xfel` will find the device and attempt to communicate with it, but since the bootloader is not listening, the message will fall on deaf ears. After a few unacknowledged retries, it will give up and report this error:

```console
$ xfel version
ERROR: xfel had timeout comunicating with device (device not listening, needs reset?)
```

## Use xfel read32/write32 to peek and poke
The commands `xfel read32` and `xfel write32` can be used to peek and poke values in memory. Admittedly these commands are more of a cute party trick than a useful way to control the Pi.

Try out these commands on the address of a GPIO peripheral register.
The blue on-board act led is controlled by GPIO PD18. The base address of config0 register for the PD group is `0x02000090`. The function of PD18 is set in config2 at address `0x02000098`. The data register for the PD group is address `0x020000a0`, the value for PD18 corresponds to the 18th bit in the register.  The exchange below shows reading the value of config2 and data and then writing to config2 and data to configure PD18 as an output and set its value to 1. Eexecuting the commands below will turn on the blue act led on-board the Mango Pi.

```console
$ xfel read32 0x02000098          # read 32-bit word from addr 0x02000098
0x0fffffff
$ xfel read32 0x020000a0
0x00000000
$ xfel write32 0x02000098 0x100    # write 32-bit value to addr
$ xfel write32 0x020000a0 0x40000
```

The command `xfel hexdump` will read a range of memory on the Pi and print the contents.  Below shows a hexdump of memory used for the GPIO peripheral registers.
```console
$ xfel hexdump 0x02000000 100     # show 100 bytes of memory starting at addr
02000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
02000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
02000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
02000030: ff ff ff ff 66 ff 0f 00 00 00 00 00 00 00 00 00  ....f...........
02000040: 00 00 00 00 11 11 11 11 11 11 01 00 00 00 00 00  ................
02000050: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
02000060: ff ff ff ff                                      ....
```

## Use xfel to load and execute a program
Before loading a program, we must first start up the memory controller. This requires a finely-tuned bit of hardware wizardry that is supplied by `xfel`. The command `xfel ddr d1` performs the correct memory initialization sequence for the D1. You must always initialize the memory controller after reset and before running a program.

After the memory system has been initialized, use the command `xfel write` to send a binary file from your laptop to the bootloader. The bootloader will receive the file and write it to memory at the address you specify. The command `xfel exec` will start execution at that address. We load our programs to address `0x40000000`, the standard start location for RISC-V.

Try these commands out now. The commands below load/execute the `blink-actled` program from the $CS107E/bin directory:
```console
$ cd $CS107E/bin
$ xfel ddr d1                                # initialize memory controller
Initial ddr controller succeeded
$ xfel write 0x40000000 blink-actled.bin     # write program to memory
100% [================================================] 44.000 B, 23.620 KB/s
$ xfel exec  0x40000000                      # start executing program
```
The blink-actled program runs an endless loop to blink the blue on-board led.

To load and run a different program, your must first reset the Pi by briefly interrupting power. This step is necessary to reset the Pi and reenter the bootloader so it is ready to receive a new program. You can reset by unplugging and replugging the USB or clicking the tiny reset button (available on Mango Pi latest board rev Dec 2025).
{% comment %}Your parts kit includes a USB cable with a rocker switch so you can reset the Pi with a quick flip off and back on. {% endcomment %}

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

If you invoke `mango-run` with name of a binary program as an argument, it will send that to the bootloader and execute it:

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

Use `xfel version` (or `mango-run` with no arguments) to test communication with the bootloader. If the response is:
- `AWUXBFEX ID=0xID_NUMBER(D1/F133) ...`
    - You are good to go! It found the connection to the Pi, the bootloader is running, and the Pi responded with its id.
- `ERROR: xfel found no connected FEL device`
    - xfel did not find a connected Pi.
	- Double-check the Pi is powered up and connected to the OTG port using a USB cable that supports both charge and data.
	- In some situations, a connection that was fine a moment ago can stop working, such as when your laptop's usb system goes a bit wonky and suspends the port.
    - Unplugging from your laptop USB port and replug may prompt your laptop to reset the port or try plugging into a different USB port on your laptop.
    - If still not resolved, reboot your laptop to set things right.
- `ERROR: xfel had timeout comunicating with device (device not listening, needs reset?)`
    - xfel found a connected Pi, but was not able to communicate with the FEL bootloader.
	- This means the bootloader is not running/listening. If you have already bootloaded a program, that program is now executing on the Pi and the bootloader is no longer running.
    - Reset the Pi to re-enter the bootloader.
- `ERROR: xfel cannot connect to usb device: -12`
    - If running on WSL, this error indicates a problem with your installation. Review the section "Install xfel" in the [install guide for WSL](/guides/install/devtools-wsl).

## Resources
- The `xfel` source code is published on github, we maintain a custom fork at <https://github.com/cs107e/xfel>
- Documentation on `xfel` is available at <https://xboot.org/xfel/>
- The `mango-run` script is available in `$CS107E/bin`.
