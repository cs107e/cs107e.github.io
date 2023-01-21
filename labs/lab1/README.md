---
released: true
permalink: /labs/lab1/
title: 'Lab 1: Setup your Raspberry Pi'
toc: true
readings: |
  Please do the Lab 1 [prelab preparation](/labs/lab1/#prelab-preparation) before coming to lab.
---

{% comment %}
Task list to copy/paste when creating PR for this lab:

__Before releasing lab1:__
- [ ] Update firmware to latest from Broadcam
- [ ] Review writeup/code/checkin questions (instructor)
- [ ] Walk through (SL)
- [ ] Followup on issues from previous quarter postmortem (issue #380)

__To prep for lab1:__
- [ ] Review tips recorded in private/staff_notes.md and private/answers.md
- [ ] Confirm installation instructions are up-to-date
- [ ] Forum announcement for students to bring adapter if their laptop doesn't have USB-A port A (e.g. recent Macs)
- [ ] Confirm multimeters have fresh batteries
- [ ] Confirm lab cabinet has SD card readers and spare C-to-A adaptors
- [ ] Prepare some SD cards with bootloader in case students can't prep their own
- [ ] Bring blank labels and pens -- students make name tag and decorate kit box

{% endcomment %}

*Lab written by Pat Hanrahan, updated by Julie Zelenski*

## Goals

During this lab you will:

- Explore simple breadboard circuits using LEDs, resistors, and buttons.
- Practice with building and running programs on the Pi.
    - Build programs using the cross-development tools you installed on your computer
    - Configure an SD card with needed firmware for your Pi
    - Download and run program on the Pi using the bootloader and rpi-run.py
- Revisit the assembly code for the blink program and play with it a little.

## Prelab preparation

To prepare, please do the following before coming to lab:

1. Organize supplies to bring with you:
    - Bring your __laptop__, ideally with full charge.
    - In this week's lab, we will give out your __Raspberry Pi kit__. The components are packed in a handy plastic box for convenient carrying. You will bring your parts kit to every lab.
    - You will need an open USB-A port on your laptop to connect to the Pi. If your laptop requires a __USB hub or C-to-A adapter__, pack one in your parts kit so that you'll always have it with you.
    -  If you have access to a __multimeter__, bring it along.
1. Reading to do ahead of lab:
    - Read this SparkFun tutorial on [using a breadboard](https://learn.sparkfun.com/tutorials/how-to-use-a-breadboard#history). Pay special attention to the section labeled "Anatomy of a Breadboard" to learn about the internal connections.
    - From our course guides, please review:
        -  [powering](/guides/power) the Pi
        -  [using SD cards](/guides/sd) on the Pi
        -  [using the bootloader](/guides/bootloader) to send programs to the Pi
        -  [one page of ARM](/guides/arm) assembly

## Lab exercises

To help with budgeting your time, we have ordered the exercises to put the critical tasks first.

- Exercises 0 - 7 are "Essential". You need to have successfully completed these tasks to be ready for Assign 1.
- Exercise 8 (Button) is a "Nice to Have". You won't need buttons until Assign 2. If you don't have time to get to this exercise during lab1, fine to get back to later on your own time or during OH.


### 0. Pull lab starter code

Before starting a new lab or assignment, do a `git pull` in your $CS107E repository to ensure the courseware files are up to date.

```console
$ cd $CS107E
$ git pull
```

Now cd to your local `mycode` repo and pull in the lab starter code:

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull --allow-unrelated-histories starter-code lab1-starter
```

Your repo should now be on the `dev` branch and should contain a new subdirectory named `lab1` containing the starter files.

### 1. Inventory your kit

You will receive your CS107e Raspberry Pi kit when you arrive at lab. Open it up and check it out!
Unwrap and discard any packaging. Try to identify what each component is and compare to the [kit inventory](/guides/bom/) to ensure your kit is complete. If you are missing any parts, ask us for replacements.

Decorate a sticker with your name to label your kit to help keep the two of you united.

### 2. Breadboard LED circuit

Get the breadboard from your kit. Review how the breadboard is internally constructed.  Which holes are connected to which other holes?  How are the power and
ground rails connected? Try using a multimeter to verify the internal connections. Grab two male-male jumpers from the lab cabinet and plug one end of each into two holes on the breadboard. Confirm that that those two holes are connected (or not) by applying the multimeter leads to the free ends of the two jumpers and testing for continuity.

You are going to wire up a simple circuit to light an
LED. Pick out a LED and a 1K resistor from your kit. A resistor's value is [marked by its colored bands](https://learn.sparkfun.com/tutorials/resistors#decoding-resistor-markings). What are the band colors for 1K? Measure the resistance using a multimeter to confirm the value.
(Recall that a [resistor](https://learn.sparkfun.com/tutorials/resistors)'s band colors tell its resistance. Check out this [chart and calculator](http://www.digikey.com/en/resources/conversion-calculators/conversion-calculator-resistor-color-code-4-band).)

Make a circuit on your breadboard with the LED and resistor. 
Remember than an LED is directional -- the longer lead is the anode and the
shorter lead is the cathode.  The voltage from anode to the cathode
should be positive.  If the polarity of voltages are switched, the LED
will not light up.  The LED also needs a current limiting resistor
otherwise it can literally [blow up in a fiery, smoky
extravaganza](https://www.youtube.com/watch?v=WLctUO1DGtw)!

In the photo below, the cathode of the LED connects to one end of the
to resistor and the other end of the resistor connects to the
the blue ground rail.  The LED crosses over the middle of the
breadboard.  

![Led and resistor on breadboard](images/led-resistor.png){: .zoom}

We are just playing with this circuit in lab, so we won't worry about making things tidy and secure. When installing a more permanent circuit such as for assignment 1, use pliers to make a clean bend in the legs and snip the ends with a cutter so that each component sits neatly on the breadboard.

To light up the LED, you need to apply power to the anode
and complete the circuit by connecting the cathode to GND.   The power will 
come from your laptop using a __CP2102 USB to Serial Adapter__
(hereafter referred to as just "USB-serial").  This is the small black
breakout board with a USB-A connector on one side and a 6-pin header
on the other side.  

Find the USB-serial in your parts kit. Plug it into a USB-A port on
your laptop (have a hub or C-to-A adapter on hand if neded). Once plugged in, a small LED on the USB-serial lights to show power is flowing. After confirming it powers up, unplug it from your laptop.

You can use the USB-serial to power the circuit on your breadboard. Follow these steps __in order__:

1. Pick out two female-male jumpers, one red and one black. When wiring, people experienced in electronics choose the color of the wire to indicate what type of signal is being carried.  This makes it easier to debug connections and visualize what's going on. Here are the conventions we adopt:

    -   Black (GND)
    -   Red (5V)
    -   Orange (3.3V)
    -   Blue (host output)
    -   Green (host input)

    The two jumpers you are using in this exercise are for power and ground, so red and black are the right choices.

1. Connect the female ends of the jumpers to the header pins on the USB-serial: the black jumper to the header labeled GND and the red to the one labeled +5V or VCC. The pin layout on your USB-serial may be different than the one in the photo: pay attention to what the pins
are labeled, not their physical location. (click photo to enlarge)

    ![USB serial with jumpers](images/usbserial.jpg){: .zoom}

1. Plug the male ends of the jumpers into the breadboard: the black jumper to the blue ground rail and the red jumper to the anode of the LED.

    ![Complete circuit](images/led-circuit.jpg){: .zoom}

1. After double-checking that your circuit wiring is correct, you're
ready to apply power. Plug the USB-serial into
your laptop and the LED on your breadboard should turn on.

While the LED is lit, make the following measurements with the multimeter. Grab a multimeter from the lab cabinet if your table doesn't have one.

-   Measure and record the voltage across the resistor
-   Measure and record the voltage across the LED

Apply Ohm's law to these measurements to calculate the current flowing through the LED.

If you substitute a 10K resistor in place of the 1K, how will this change the rate of current flow?  What effect does this have on the brightness of the LED? Grab a buddy and do a comparison! Have one of you swap in a 10K resistor in place of 1K (be sure to disconnect power before fiddling with the wiring!). Compare the brightness of the changed circuit to the original and see the effect with your own eyes.

You are ready to answer the first check-in question. [^1]

### 3. Power via the Pi
Get your Raspberry Pi from your kit. Identify the 40-pin GPIO header along the long edge. Orient the header to match the [pinout diagram](/guides/images/pinout.pdf). The pinout is also available as a postcard in your kit and a poster on lab wall. We even have a command-line version that display in retro ascii art. Try it out now!
```console
$ pinout
```
Use the pinout to identify two 5V power pins and two GND pins on your Pi; you'll use these pins in this exercise.

You are going to re-wire your circuit to run power/ground from the USB-serial first to the Raspberry Pi and from there to the breadboard.  Follow these steps __in order__:

1. First, unplug the USB-serial from your laptop so that no power is flowing. 
    >**Danger** Always have the USB-serial unplugged from your laptop
    whenever you are fiddling with the wiring.
    If you leave it plugged in, power is flowing and all wires are live, which makes for a dicey situation. An accidental crossed wire can a short circuit, which could fry your Pi or make your
    laptop disable the USB port.
    {: .callout-danger-invert}

2. Disconnect the USB-serial from the breadboard and connect instead to the Raspberry Pi. Pick out two female-female jumpers, one black and one red. Black connects GND on the USB-serial to a GND GPIO on the Raspberry Pi and red connects 5V(VCC) on the USB-serial to a 5V GPIO.

3. Connect power and ground from the Raspberry Pi to the breadboard using the two female-male jumpers.  The black jumper connects a GND GPIO on the Raspberry Pi to the blue ground rail on the breadboard. The red jumper connects a 5V GPIO to the LED anode.

4. After double-checking your wiring, apply power by plugging the USB-serial in your laptop. All three LEDs should light: the one on the USB-serial, the red PWR LED on the Raspberry Pi, and the LED on the breadboard. Power is flowing from the USB-serial to the Raspberry Pi and through to the breadboard. 

    ![Complete circuit](images/piled.jpg){: .zoom}

### 4. Prepare SD card
Your parts kit contains an microSDHC card. A micro-SD is shown below with its adapter "jacket" . The Raspberry Pi card reader slot accepts an un-jacketed micro-SD card.

![](/guides/images/sd.kingston.jpg){: .w-25}

When the Pi reset, it looks for a program file named `kernel.img` on its SD card. To change the program executed after reset, you change that file.

You will prepare your SD card with the firmware files needed for the Raspberry Pi by copying the files from our courseware repo.

Follow these steps:

1. Find the slot for the built-in card reader of your laptop or plug in an external card reader if no built-in.  We have external card readers you can borrow if needed. Insert your SD card into the card reader. Note some card reader slots are sized such as to require the micro-SD to be in a jacket.  Once inserted, the SD card should mount automatically and the volume shows up in the macOS Finder or Windows File Explorer.  By default, the volume is named `NO NAME`. You can change the name if you wish.

2. Confirm the firmware folder of our course repo contains these four files:

    ```console
    $ ls $CS107E/firmware
    blink-actled.bin   bootloader.bin  bootcode.bin       start.elf
    ```

    The `bootcode.bin` and `start.elf` files are needed for the Raspberry Pi reset sequence. The two additional files
    `blink-actled.bin` and `bootloader.bin` are programs.

3. Copy the four files from the firmware folder onto the mounted SD card volume.
    - On macOS, the SD card volume is mounted under the parent directory `/Volumes`. Change to the card volume and copy the firmware files here:

        ```console
        $ cd /Volumes
        $ ls
        Macintosh HD    NO NAME
        $ cd NO\ NAME
        $ cp $CS107E/firmware/* .
        ```
        {: .console-mac}

    - WSL does not have access to the mounted volume so you must use the Windows File Explorer.  Use the commands below to open the firmware directory in File Explorer and then select the four files from the firmware folder and copy them to the mounted SD card.
        ```console
        $ cd $CS107E/firmware
        $ explorer.exe .
        ```
        {: .console-ubuntu}

4. The SD card needs an additional file named `kernel.img`. 
    Normally, `kernel.img` is the operating system kernel you want to run, like Linux or Windows. In this course, we will write our own program to take the place of the kernel, and put our program under the name `kernel.img`. One of the firmware files is a program called `blink-actled.bin` that blinks the green activity (ACT) LED on the Raspberry Pi board. We are going to use this blink program as the kernel for a test.

    On the SD card, make a copy of the `blink-actled.bin` file and name it `kernel.img`.  
3. Confirm that the SD card contains these essential reset files: `bootcode.bin` `start.elf` and `kernel.img`. The card can also contain additional files; any other files are ignored.

4. Eject the SD card in the Finder/File Explorer.  If Terminal prevents you from ejecting, use the command `cd ..` to move to the parent folder and try ejecting again.

5. Insert the SD card into the slot on the bottom side of the Raspberry Pi board. The slot has a small spring that holds the card in place. As you push the card in, you will feel the mechanism grab onto the card and secure it.

    ![](/guides/images/sd.pi.jpg){: .zoom .w-25}

6. Power up the Pi.  The green ACT LED on the Pi's board
should slowly blink.

Keep this procedure for preparing a fresh SD card in the back of your mind.
If you ever think your Pi is not working because of a hardware problem,
repeat these steps, using the blink program to confirm the basic operation.

There is additional advice on troubleshooting SD cards and the Pi reset sequence in our [guide to SD cards](/guides/sd#troubleshooting).

### 5. Blink breadboard LED
The `kernel.img` file currently on the SD card is a copy of the `blink-actled` program which pulses the on-board ACT LED.  Each time the Pi is reset, it runs this same program. To change which program is run, we must update the SD card with a different program. Let's go through that process now.

You want to run `blink` program from lecture which blinks GPIO 20 on and off. Re-configure your breadboard circuit for this program by connecting GPIO 20 to the anode of the LED.  Be sure that the USB-serial is unplugged before rewiring!  Use the [pinout](/guides/images/pinout.pdf) to identify which header pin is GPIO 20.

The `blink` subfolder of `lab1` contains the code for the blink program shown in lecture. Change to that folder and build the `blink` program using these commands:
<a name="build"></a>
```console
$ cd lab1/blink
$ arm-none-eabi-as blink.s -o blink.o
$ arm-none-eabi-objcopy blink.o -O binary blink.bin
```
The `arm-none-eabi-as` command assembles the ARM instructions in `blink.s` into an "object
file", a particular file format for binary code. An object file has a
lot of information in it that we don't need -- we just want the
program.  The final command `arm-none-eabi-objcopy` turns this
object file into a raw binary program of instructions named
`blink.bin`. To run this program on
the Pi,  we need to update the files on the SD card to use the `blink.bin` program as `kernel.img`.

Power down the Pi, and gently remove the SD card from the underside.

>**Do not force the mechanism!** When removing the SD card from the Pi's card slot, gently push the card in and allow it to spring back out. If you try to pull out the card by force, you can break the mechanism and potentially destroy your Pi.
{: .callout-danger-invert}

Mount the SD card  on your laptop. Copy the `blink.bin` file that you just built to the card and name it `kernel.img`, replacing the file that was previously there. Eject the SD card from your laptop and re-insert the card into the Raspberry Pi.

Power up your Pi and it now runs the `blink` program, which blinks the LED on your breadboard.

![GPIO 20 connected to LED](images/piled.gpio20.jpg){: .zoom}

You have just run your own bare metal program on the Raspberry Pi -- hooray!

### 6. A better way: bootloader

Each time you change your code, you could repeat this process.
You would
power down your Pi, remove the SD card,
mount the SD card on your laptop,
copy the new version of your code to `kernel.img`, eject the SD card from your laptop,
insert it into the Pi,
and then power it up.
This quickly becomes tedious.
Even worse, the SD connectors are only designed to withstand
around 1000 insertions and deletions, after which they start to fail.

Instead, you can set up a serial connection between your laptop and the Pi and use a __bootloader__ to transfer the program. The bootloader is a program that runs on the Pi
and listens on the serial connection. On your laptop, you run a script to send the program to the waiting 
bootloader. The bootloader receives the program and writes it to the
memory of the Pi, a process called "loading" the program. After the
program is loaded, the bootloader jumps to the start address of the program,
and the program begins to run.

To stop that program and start another, reset the Pi and use the bootloader again.  This is much mroe conveinent way to run your newly compiled program than all that shuffling of SD cards. You will learn to love the bootloader!

Here is how to use the bootloader program as `kernel.img`:

1. Mount the SD card on your laptop. The SD card should already contain a file called `bootloader.bin` (it was one of the firmware files copied in exercise 4). Make a copy of
   `bootloader.bin` and name it `kernel.img`, replacing the program
   that was previously there.

2. Eject the SD card and insert it into the Raspberry Pi. The bootloader will now run any time you set the Pi.

To use the bootloader, you must set up the communication channel between your computer and the Pi.  The USB-serial contains pins that can be used as a serial communication line.

The 6-pin header at the end of the USB-serial breakout board has two pins labeled for transmitting (TX) and receiving (RX).
The Pi has GPIO pins designated for TX and RX. Use the [pinout](/guides/images/pinout.pdf) to find the TX (GPIO pin 14) and RX (GPIO pin 15) pins on the GPIO header.

As always while re-wiring, unplug the USB-serial so no power is flowing. Pick out two female-female jumpers, one blue and one green. Use the blue jumper to connect the __TX__ of the USB-serial to the __RX__ on the Pi, and the green jumper to connect __RX__ of the USB-serial to the __TX__ on the Pi. The correct connections are shown in the photo below (click to enlarge):

![Console cable](/guides/images/usb-serial-bootloader.jpg){: .zoom}

>__Careful with the connections__ The connections run from one device's TX to the other's RX, i.e. the data __transmitted__ by your laptop is __received__ by your Pi and vice versa. Connecting TX to TX and RX to RX is not correct! Also note that the pins on your USB-serial
may be in different positions or have different label names. Don't just follow the photo blindly.
{: .callout-warning}

After confirming your wiring is correct, plug in your USB-serial to power your Pi. The bootloader should run on reset. When the bootloader is running, it signals that it is waiting to receive a program by repeatedly giving two short flashes of the green ACT LED on the Pi board. This "da-dum" is the heartbeat that tells you the bootloader is ready and listening. Look at your Pi now and observe this rhythm. Breathe in sequence with it for a moment to celebrate having achieved bootloader readiness.

Our Python script `rpi-run.py` runs on your laptop to send a program to the bootloader. Verify you have __Version 2.3__ using the following command:

```console
$ rpi-run.py -h
usage: rpi-run.py [-h] [-v] [-q] [-t T] [-p | -s] [port] file
    
This script sends a binary file to the Raspberry Pi bootloader. Version 2.3
...
```

Let's try bootloading a program. Change to the `lab1/blink/`
directory where you built `blink.bin`.

The `rpi-run.py` command is used to send a program to the bootloader. Try it now:

```console
$ rpi-run.py blink.bin
Found serial device: /dev/cu.SLAB_USBtoUART
Sending `blink.bin` (72 bytes): .
Successfully sent!
```

While receiving a program, the bootloader turns on the ACT LED and holds it steady until transmission is complete. The bootloader then turns off the ACT LED and transfers execution to the received program. The `blink.bin` program will pulse GPIO 20 which is connected to the LED on your breadboard.

If you change your program and want to run it again, you must first 
reset the Pi. What happens if you try to `rpi-run.py` a second time
after the bootloader has already loaded a program? Why does that happen?

One way to reset the Pi is to briefly cut power by unplugging the USB-serial
from your laptop,
and then plug it in again.
The Pi will restart into bootloader, ready to receive a new program. 

Reset your Pi now and re-run the blink program. Hoorah, hoorah, hoorah!! 👏 Show off your working edit-build-run cycle when you check-in with us. [^2]

### 7. Study the blink program

Below is the blink program that Pat wrote in Monday's lecture. This code is available in the file `lab1/blink/blink.s` and also reproduced below.

```
.equ DELAY, 0x3F0000

// configure GPIO 20 for output
ldr r0, FSEL2
mov r1, #1
str r1, [r0]

mov r1, #(1<<20)

loop: 

// set GPIO 20 high
ldr r0, SET0
str r1, [r0] 

// delay
mov r2, #DELAY
wait1:
    subs r2, #1
    bne wait1

// set GPIO 20 low
ldr r0, CLR0
str r1, [r0] 

// delay
mov r2, #DELAY
wait2:
    subs r2, #1
    bne wait2

b loop

FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C
```

If there is anything you don't understand about this program,
ask questions of your partner and others.

Edit the delay constant to make the delay to be half as long. Rebuild and run the program to see that it now blinks twice as fast. Modifying the program is a multi-step process. First you edit `blink.s` in a text editor, then use the same [build commands](#build) as previous to rebuild `blink.bin` . To run the new program, first reset your Pi by unplugging and replugging, then use `rpi-run.py blink.bin` to run your updated program on the Pi. Make sure you understand why these steps are all necessary.

Experiment with changing the value of delay to achieve a blink rate of roughly 1 second on and 1 second off. Using that value you can calculate an estimate how many instructions per second the Raspberry Pi is executing. You can now answer this check-in question[^3].

<A name="button"></A>
### 8.  Adding a button
The final lab exercise is to study the `button` program and build a breadboard circuit to test the program. This button program is in the file `lab1/button/button.s`.

The button program reads the state of a button connected to GPIO 10 and turns off the LED on GPIO 20 when the button is pressed.

```    
// configure GPIO 10 for input
ldr r0, FSEL1
mov r1, #0
str r1, [r0]

// configure GPIO 20 for output
ldr r0, FSEL2
mov r1, #1
str r1, [r0]

// bit 10
mov r2, #(1<<10)

// bit 20
mov r3, #(1<<20)

loop: 
    // read GPIO 10
    ldr r0, LEV0
    ldr r1, [r0]
    tst r1, r2
    beq off // if button is pressed (reads LOW), goto label "off"

        // set GPIO 20 high (turn on LED)
    on:
        ldr r0, SET0
        str r3, [r0]
        b loop

        // set GPIO 20 low (turn off LED)
    off:
        ldr r0, CLR0
        str r3, [r0]
        b loop


FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C
LEV0:  .word 0x20200034
LEV1:  .word 0x20200038
```


Challenge yourself to understand
what each line of code accomplishes and why it works as expected.
You may want to add your own code annotations as you figure out each line. Have the [Broadcom peripheral manual](/readings/BCM2835-ARM-Peripherals.pdf) handy for looking up information about the peripherals. And here is the documentation for the [ARM instruction set](/readings/armisa.pdf).

Here are a few questions to test your understanding.

-  What information is stored in peripheral register at address 0x20200034?
-  What is the relationship between the `SET0`, `CLR0`, and `LEV0` registers?
-  What value is being tested by the `beq` instruction?

With this knowledge, you're ready to answer the final check-in question[^4].

Now that you understand how the program operates, you can reconfigure your breadboard circuit to test the program. 

Grab a pushbutton mechanism and button cap from your parts kit. The button has four legs. The legs are partitioned into two pairs of legs, where the pair is always connected. When the button is pushed, all four legs become connected. Your first task is to work out which pairs are always connected and which only become connected when the switch is closed. How the legs are wired is not obvious! A good way to experimentally confirm is to measure resistance/continuity across the legs using a multimeter.  Once you understand how the legs are connected,  position the button on the breadboard so it can act as a switch.

The last detail to work out is the default state of the input pin used to read the button state. Before we connect the pin to something, its voltage is in a "floating" state. It's up to us to intentionally pull the pin to a known initial state so we get a reliable reading. The button program above is written to expect an initial state of high. We will connect the pin to the power rail via a 10K resistor and "pull up" the line to set its initial state as high. GPIO 10 will initially read as high. When the button is pressed, it grounds the circuit and GPIO 10 will read low.  Sparkfun has a nice tutorial on the [use of pull-up resistors](https://learn.sparkfun.com/tutorials/pull-up-resistors/all) for more information.

Here is the schematic. VCC is 3.3V and R1 is 10K.

![Button with pull-up resistor diagram](images/pull-up-resistor.jpg)
{: .w-50 .mx-auto}

Add the above circuit on your breadboard and use GPIO 10 as your input pin. Use your pinout to find GPIO 10 on the Raspberry Pi header. Be sure to include the 10K resistor! Without that resistor, pressing the button would create a short between power and ground and could damage your Pi.

After confirming your circuit is correctly constructed, power it up and run the button program. In the initial state, the LED is on. When you press and hold the button, the LED turns off.

## Check in with TA

Each table group should periodically touch base as you answer the check-in questions below. The check-in allows us to
verify your understanding and help with any unresolved issues. We encourage you to check-in as you go, after each question, rather than batching them up at the end.

Remember that the goal of the lab is not to answer exactly and only these questions -- it's to work through the material. The questions are an opportunity to self-test your understanding and confirm with us.

[^1]: How much current flows through the LED with a 1K resistor? With a 10K?
[^2]: Show us that you have a working cycle for edit/compile/execute program on your laptop and Pi. We want to be sure everyone leaves lab with this success under their belt!
[^3]: What is your experimental estimate of the number of instructions per second executed by the Raspberry Pi?
[^4]: Explain the relationship between the `SET0`, `CLR0`, and `LEV0` registers.

It's okay if you don't completely complete all exercises during
lab; your sincere participation for the full lab period is sufficient
for credit.  However, if you don't finish, we highly encourage you to
work those parts to solidify your knowledge of this material before
moving on.

### Clean up
Before leaving lab, be sure to return our tools and supplies to their rightful place in the cabinet, discard any trash from your table, and straighten up the tables and chairs. Our lab room is our home, let's all work together to keep it a tidy and welcoming place!

<style>
    .footnote:before {
        content: ' Q-';
    }
    .footnotes:before {
        content: "Check-in questions";
        font-size: 125%;
    }
</style>

