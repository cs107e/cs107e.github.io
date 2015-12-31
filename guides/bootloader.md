## Working with the bootloader

*Written by Pat Hanrahan*

In the [SDHC card guide](../sd.md), you should have installed a 
bootloader on the SDHC card as `kernel.img`.
When the Raspberry Pi runs, the bootloader is run.
The bootloader listens on the serial port
for commands to load a program into memory on the Pi.
After the program is loaded,
the bootloader jumps to the start address of the program,
and the program begins to run.
If you want to run a new version of your program,
you reboot the processor,
and download a new version of your program.

So each time you wish to run a new version of your program, you only
need to do two steps:

1 Reset the Raspberry Pi

2 Run the command to load and start your program

In order to use the bootloader, 
you need to format an SDHC card 
with the bootloader installed as `kernel.img`.
You also need to connect your console cable
to your laptop and the Raspberry Pi.
Finally, you need to run a script that uses
the `xmodem` protocol to send the program to the Pi.

### Install the bootloader

The bootloader we are using was developed by David Welch and 
modified by Dawson Engler.
It is a modification of David Welch's bootloader06 from
(https://github.com/dwelch67/raspberrypi).
If you have some time,
we highly recommend you explore his github repository.

We have already compiled a working `bootloader.bin` for you.
It is part of the default set of firmware files,
and should already be on your SDHC card.
To run the bootloader, just copy `bootloader.bin` to `kernel.img`.
Next time you reset the Raspberry Pi,
the bootloader will run.

### Connect your laptop to the Pi using the console cable

Setup your console cable as per our [instructions](console.md)

### Load and run your program 

We have created a python program that sends binary
files to the bootloader.
If you have followed all the setup instructions,
the program is already installed. If you are on a Mac and 
it is not working for you, be sure you followed the latter part
of the setup instructions [here](mac_toolchain).

To load and run `blink.bin`, simply type:

    % rpi-install.py blink.bin
    Sent True
    %

After a few seconds, you should see the LED blinking.

