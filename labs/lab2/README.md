---
released: true
permalink: /labs/lab2/
title: 'Lab 2: Below C Level'
toc: true
readings: |
   [Prelab](/labs/lab2/#prelab-preparation) to prep before lab.
---
{% comment %}
Task list to copy/paste when creating PR for this lab:

__Before releasing lab2:__
- [ ] Review writeup/code/checkin questions (instructor)
- [ ] Walk through (SL)
- [ ] Followup on issues from previous quarter postmortem (issue #)

__To prep for lab2:__
- [ ] See private/answers and private/staff_notes for advice saved from past
- [ ] Piazza announcement for students to bring their own tools (also repeated in prelab)
- [ ] Confirm lab cabinet has plenty of hookup wire and hand tools
- [ ] Make copies of 7-segment pinout diagram to distribute in lab

{% endcomment %}

![Death Valley](images/belowClevel.jpg){: width="50%" style="float:right;"}

<br/>
*Lab written by Pat Hanrahan*

---
<br/>
## Goals
During this lab you will:
- Understand the assembly language produced by `gcc` when compiling a C program.
- Understand basic makefiles.
- Learn how to unit test your C program.
- Begin breadboarding a 4-digit 7-segment display for your next assignment, the clock.

## Prelab preparation

To prepare for this lab, please do the following:

1. Read our [gcc guide](/guides/gcc)
   about how to compile C programs for bare metal
   programming on the Raspberry Pi.

2. Read our [make guide](/guides/make)
   on the structure of makefiles.

3. Read [section 4a below](#crossref) on the theory of operation
   for 7-segment displays. If you have time, skim the rest of exercise 4 to get the lay of the land for the breadboarding work.
    - You will be doing a lot of breadboarding in this lab and using hand tools such as wire strippers, cutters and pliers.  We will have tools available, but there is not enough for everyone, so you will have to share them. __If you have your own tools, please bring them to lab!__
    - [Pat's advice](/guides/handtools/) on owning your own tools.

4. Pull the latest version of the `cs107e.github.io` courseware repository.
5. Clone the lab repository `https://github.com/cs107e/lab2`.


## Lab exercises

First things first: get some snack and pair up with a buddy and introduce yourself. Congratulate each other on completing your first assignment! Bring up the [check-in](checkin) questions to ponder as you work through lab. There are four exercises today and we recommend that you spend no more than 20 minutes each on the first three, reserving the second hour of lab for the breadboarding task. You should leave lab with a solid start on the hardware setup needed for your next assignment.

### 1. C to assembly (20 min)

Compilers are truly an engineering marvel. Converting a C program into a fitting use of assembly instructions, registers, and memory necessitates both technical mastery and a fair bit of artistry. From here forward in the course, you'll hand over to the compiler the task of writing assembly, but you will continue to grow your fluency reading and understanding assembly. For this exercise, you will observe and pay tribute to the handiwork of the compiler.

Sometimes the assembly
produced by the C compiler can be surprising. You will be pleased by its clever optimizations, although occasionally its eagerness can also remove or rearrange your code in ways that confound your intentions. When this happens to you, you can deploy your ARM superpowers to dig into the generated
assembly and figure out what the compiler did rather that sit
there dumbfounded by the unexpected behavior!

Change to the `lab2/code/codegen` directory. Open the `codegen.c` source file in
your text editor. The file contains four parts that concern different aspects of C: arithmetic, if/else, loops, and pointers.

Skim the C code and read our comments.

In your browser, visit the [Compiler Explorer](https://godbolt.org/z/fztCXO). Configure the settings to match our toolchain:  `C` language, compiler version `ARM gcc 5.4.1 (none)` and optimization flags `-Og`.

Copy the  `part_a` function
from `codegen.c` and paste into the Compiler Explorer source pane on the left and read the generated assembly shown in the right pane. Verify that the assembly accomplishes what was asked for in the C source. Do you note any surprising choices in how it goes about it?  Read our comments in `codegen.c` as the guide for what to look for and follow-up experiments to try. After you finish exploring `part_a`, do the same with parts b, c, and d.

The Compiler Explorer is great fun for a quick interactive exploration; you can also use your regular command-line tools to see the compiler's output. The command `arm-none-eabi-objdump -d somefile.o` invokes the _disassembler_ to extract the assembly from a compiled object file. Run the command `make codegen.list` to see the steps involved. Open the resulting `codegen.list` file in your text editor and you can review the entire program's worth of assembly.

A good way to learn how a system works is by trying
things. Curious about a particular C construct is translated to assembly? Wonder about the effect of changing the compiler optimization level? Try it out and see. Let your curiosity be your guide!

### 2. Makefiles (20 min)

Change to the directory `lab2/code/makefiles` and view the C version of the blink program and its simple Makefile, reproduced below:

```
    NAME = blink

    CFLAGS = -g -Wall -Og -std=c99 -ffreestanding
    LDFLAGS = -nostdlib -e main

    all: $(NAME).bin

    %.bin: %.elf
        arm-none-eabi-objcopy $< -O binary $@

    %.elf: %.o
        arm-none-eabi-gcc $(LDFLAGS) $< -o $@

    %.o: %.c
        arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

    install: $(NAME).bin
        rpi-install.py $<

    clean:
        rm -f *.o *.elf *.bin
```

Discuss and document the various features and syntactical
constructs used in this Makefile.

 - What is the purpose for each of the CFLAGS?
 - What happens if you just type `make`? Which commands will execute?
 - If you modify blink.c and run `make` again, which commands will rerun?
What part of each target indicates the prerequisites? (A prerequisite means
that if that file changes, then the target is stale and must be rebuilt)
 - What do the symbols `$<` and `$@` mean?

You should be able to answer the [make check-in question](checkin) now.

### 3. Testing (20 min)

An effective developer knows that testing your code goes hand-in-hand with writing it. The better your tests and more timely your efforts, the sooner you will find your bugs and the easier your debugging will be. To help you grow this important skill, upcoming assignments will include a required testing component along with our guidance on testing structure and strategies..

The standard C library offers an `assert` macro for use as a simple diagnostic. Run the command `man assert` in your Terminal to read about the standard library version. The `assert` macro takes an expression that is expected to evaluate to true. If the expression is, in fact, true, then the assertion succeeds and the program continues on.  If the expression is false, the assertion fails which cause the program to print an error message and exit.

Running bare metal means no standard libraries, and furthermore we don't have printf (yet!). To get something akin to `assert`, we have to cobble it up ourselves and given our limited resources, it will be rather primitive.  There are two LEDs built onto the Raspberry Pi board that we will use to signal success and failure. One is a red LED labeled "PWR"; by default this LED is lit whenever the Pi is receiving power. There is also a green LED labeled "ACT" that we have used by the bootloader heartbeat.  Our bare bones `assert` is going to co-opt these LEDs for its own purposes and blink the red LED to report a failure and turn the green LED steady on to report total success. 

Let's walk through an example that shows using assertions as rudimentary testing tool.

#### A buggy program

Change to the `lab2/code/testing` directory. The program in `testing.c` defines the `count_bits` function to count the on bits in a given number.  Each test case in `main()` calls `count_bits` on an input and asserts that the result is correct. The code in `count_bits` works correctly for some inputs but not all. Don't try to work out the bug by inspection, instead let's see how we can using testing to determine which test cases are failing.

#### What do you expect?

First, let's review what we expect to
happen when executing the test program. If a test case fails, the `assert` macro (in `assert.h`) will call `abort`, but what does `abort` do? (hint: read the comments in the file `abort.c`)

Look at `cstart.c` to see what happens after the
program runs to completion without a failed assert, i.e., what
follows after `main()` finishes?. (Don't worry about the `bss` stuff
for now: we will talk about that in lecture soon.)

What do you expect to see on the Pi if:
- if the `count_bits()` test case is called on a value that triggers its bug?
- if the `count_bits()` test case is called on a value that works correctly?
- if the executing program has multiple test cases, some of which succeed and some that fail?

#### Run tests

Use `make` to build the program as-is and `rpi-install.py testing.bin` to execute on the Pi. You get the blinking red LED of doom. Hmmm, so at least one test failed, but which one?

It's time to divide and conquer. Leave in the first half of the test cases and comment out the remainder. Rebuild and re-run. If it fails, you know you have a culprit(s) in the first half; otherwise you can move on to looking in the second half. The strategy is to iterate, selectively commenting in/out
test cases and re-running to narrow in on which specific cases fail.
How many of the test cases pass? How many fail?  Which ones? Why?

Organize the test results to identify the pattern to the failures. Use that information to find and fix the bug in `count_bits` so that it works correctly for all inputs. Note that a test case is only as good as you make it. An incorrect test case could make you think your code is working when it isn't, or make you think your code isn't working when it is &mdash; so, in addition to fixing `count_bits`, go through all the test cases and make sure the test is properly constructed, i.e. properly asserts an expression that should be true.

After fixing `count_bits` and any incorrect test cases, uncomment all test cases, rebuild, re-run, and bask in the glow of the green light of happiness!

#### Make install

Phew, typing out `rpi-install.py testing.bin` so many times was
a chore! Add a recipe for
`install` to the Makefile that will build and run
the program with the single command `make install`. Make sure that the dependencies for the install target are configured to trigger a rebuild of the program when needed.


<a name="crossref"></a>
### 4. Wire up display breadboard (60 min)

The second half of the lab period is devoted to wiring up the breadboard circuit needed for Assignment 2, a clock implemented on a 4-digit 7-segment display.

The lab exercise below guides you in stages and has you test each stage before moving on.
This "test as you go" strategy is the hallmark of a great engineer. Do not cut corners in a mad rush to finish in record time! The important goal for tonight is to thoroughly understand the circuit you're building, make a solid start on it, and leave with a clear plan for how to complete the rest on your own.

#### 4a) Theory of operation

Start by understanding how a single 7-segment display works.
<span class="pull-right" markdown="1">
![7-segment](images/segments.png)
</span>
The 7-segment display, as its name implies, is comprised of 7 individually lightable LEDs, labeled A, B, C, D, E, F, and G. There is also a decimal point labeled DP. Each segment is an LED. Recall that an LED has an anode and a cathode. The polarity matters for an LED; the anode voltage must be positive relative to the cathode for the LED to be lit. If the cathode is positive with respect to the anode, the segment is not lit.

On the 7-segment displays we are using,
the cathodes (ground) are all connected together,
to a single ground.
Such a display is called a *common cathode* display.

![Common Cathode](images/common.cathode.png)

To display a digit, you turn on the appropriate segments by connecting the
common cathode (ground) to ground and applying a voltage to those segment
pins. Turning on all seven segments would display the digit `8`.

Your clock will display minutes and seconds, using two digits for the minutes and two digits for the seconds,
for a total of four digits.
The clock display in your kit has four 7-segment displays integrated into a single unit, as shown in the photo below:

<img src="images/display.jpg" title="4-digit, 7-segment display" width="200">

Here is the schematic for the four-digit 7-segment display:

<img title="7-segment schematic" src="images/display.schematic.png" width="600">

Untangling the schematic can be a bit tricky. There are twelve pins in total: four digit pins (`DIG.1`, `DIG.2`, `DIG.3`, and `DIG.4`) and eight segment pins (`A`, `B`, `C`, `D`, `E`, `F`, `G`, `DP`). Each segment pin connects to all four digits; trace from the pin numbered 11 to see the connections to the `A` segment for each digit. Each digit has its own unique ground, e.g. `DIG.1` is the cathode/ground pin for digit 1. Trace how each of eight segments connect to this shared ground.

If you turn on segment pins `B` and `C` and connect `DIG.1` and `DIG.2` to ground, the first
and second digits both display `"1"` while the third and
fourth digits do not display anything (their ground is not
connected).

The photo below shows the placement of the pins on the display. Note that the `DIG.1`,
`DIG.2`, `DIG.3`, and `DIG.4` labels have been shortened to `D1`, `D2`, `D3`, and `D4`. The pins are also numbered for reference. By convention, numbering starts at the bottom left corner (pin #1), and proceeds in a counter-clockwise fashion until reaching the upper left corner (pin #12).

![4-digit, 7-segment display](images/display.labeled3.jpg){: width="60%"}

#### 4b) Wire up resistors/segments

In steps 4b and 4c, you will connect your display pins and test
them. For ease of debugging, we recommend that you first connect your display using jumper cables. After validating your circuit, you will re-wire it in
a neater and more permanent fashion.

*Note: The wire supplied in lab is #22 AWG solid core. Some wire strippers have a
 #22 hole that is perfectly-sized for stripping this gauge wire.*

First, connect the two power rails and two ground rails on your breadboard using red and black wires. This makes accessing power and ground via jumpers more convenient. My convention is to always orient my breadboard so that the blue ground rail is on
the bottom (after all, ground is always underneath us).

![Breadboard with two wires](images/power-rails.jpg)

Now insert the display on the far right edge of breadboard. Make sure the display is
oriented correctly: the decimal points should be on the bottom, and the digits
slanted to the right.

Take note of the column numbering on your breadboard when you place your
display. Knowing which breadboard column number aligns with each pin is helpful since after you insert the display into the breadboard you can no longer
see the pins underneath. We chose to place our display so __pin 1 of the display is aligned with column 50 on the breadboard__.

The display LEDs require a current-limiting resistor just as the LEDs in your larson scanner did. Place a 1K resistor on the board bridging the
middle. Use the cutter to clip the leads so that it sits neatly.

Use a pair of red and black male-female jumpers to connect the power and ground rails of the breadboard to the 3.3V and Ground
pins on your Raspberry Pi. Pick out three short male-male jumpers (orange for 3.3V, black for GND, and green). Use the orange jumper to connect the top of
the resistor to the red power rail. Use the green jumper from the bottom of the resistor to
segment A (Pin 11, which will be at breadboard columns 51 if you aligned Pin 1
to column 50 as described above). Use the black jumper to tie digit D1 (Pin 12, column 50) to the ground rail. When you apply power to your Rasberry Pi, segment A of
digit 1 should light up as shown below:

![Wired breadboard with components](images/jumper1.jpg)

You can change which segment and digit is lit by moving your jumpers to different pins. Move
your segment jumper to light up segment B instead of segment A. Move your
digit jumper to light up segment B of digit 2 instead of digit 1. Add an
additional jumper to light up segment B of **both** digits 1 and 2.  Note
that you cannot simultaneously display different segments on different digits:
Why not?

You and you partner should now discuss the [checkin](checkin) question on the 7-segment display and confirm with the TA before moving on.

For the remainder of the lab, the breadboard samples will be accompanied by
_circuit schematics_. Below is the schematic for the breadboard circuit
shown above. Take a moment to identify all of the components in the schematic.


![Matching breadboard circuit diagram](images/schematic1.png)


Now place seven additional 1K resistors on your breadboard, bringing the total to eight, one for each segment. We
suggest the convention that the leftmost resistor controls segment A, and so
on such that the rightmost resistor controls segment DP. After you add the
resistors, test your circuit. Simultaneously wiring up all segments with 8
jumper cables can be messy; instead use a pair of jumpers to wire up 2 segments
at a time and move the jumpers to test all 8. As you go, you may want to make a
sketch of the correct connection between each resistor and its display pin you
can refer to when wiring up the permanent circuit.

Wire up your jumpers to display the pattern `"1 1 "`, just as in
the schematic below. Here a space means that the digit is blank (no segments
turned on).

![1-1- circuit diagram](images/schematic2.png)

![Wired breadboard with components](images/jumper2.jpg)

#### 4c) Wire up transistors/digits

Up to now, you have been controlling whether a digit is on by adding or
removing a jumper that connects the digit pin to ground. We eventually want to
control which segments and digits are turned on using the Raspberry Pi GPIO
pins, so we need an electronic switch that can be controlled by these pins.
To do this we will use bipolar-junction transistors, or BJTs.

A transistor has three terminals— the base (B), collector (C), and emitter (E).
The base controls the amount of current flowing from the collector to the
emitter. Normally, no current flows from collector to emitter. This condition
is an open circuit. However, if you apply 3.3V to the base, the collector will
be connected to the emitter and current will flow. This is equivalent to
closing the switch.

We will be using 2N3904 transistors. The following diagram identifies which
pins on the 2N3904 are collector, base, and emitter.

![2n3904](images/2n3904.jpg)

The transistor cap has a flat side and a rounded side. If you are looking
at the flat side with the legs pointing down, the leftmost leg will be the
emitter.

Instead of wiring a digit pin directly to ground as before, you will connect the digit pin to the collector of a transistor whose emitter is connected to ground.
Now applying power to the transistor base activates the switch and grounds the
digit pin.

First, disconnect the direct connections from your digit pins from ground.
Place the four transistors in your breadboard. We suggest arranging your
transistors such that the leftmost transistor controls digit pin D1 and the
rightmost controls pin D4.

Now connect D1's transistor. Wire the collector to D1 and the emitter
to ground. Connect the base to the control voltage through a 1K
current-limiting resistor.

Apply power to the base of the transistor.
You should see `"1 ‍ ‍ ‍ "` on the display.

Here's a board and matching schematic where we've connected _both_ D1 and D3 to
the collectors of transistors and applied power to the bases of those two
transistors. This circuit displays `"1 1 "`.

![2 transistor circuit diagram](images/schematic3.png)

![Wired breadboard with components](images/jumper3.jpg)

Test your transistor-controlled display by turning on each digit
individually to display a `1`.

#### 4d) Permanently wire circuit

Now comes the time-consuming part. Each segment pin needs to be connected to
its resistor and each digit pin connected to the collector of its transistor.
Be patient, this takes some time.

Here's a photo of what it should look like before wiring...

![Wired breadboard with components](images/parts.jpg)

...and here is a full schematic of what you will be wiring up:

![Full schematic](images/schematic4.png)

In the diagram above, the 3.3V input we've been using up to this point has been
replaced by labeled dots where you should connect jumpers from the GPIO pins on
the Pi. For example, setting Pins 10 and 20 to high will turn on the top
segment of the first digit.

The finishing touch is to add a red pushbutton on the left edge of the breadboard to be the start button for the clock. The button is not connected into the display circuit. It will be wired to the power rail through a 10K pull-up resistor and connected to the GPIO pin 2 on your Raspberry Pi to be read as an input. Review the [button exercise from lab1](/labs/lab1#button) if you were not able to get to it last week.

When you wire your breadboard, be sure to choose (or cut) wires of the proper length and arrange them neatly. Select different colors of wires to annotate what each is used for. If they're neat, it's easier to see if everything is set up correctly. Take your time and check your work. A little bit of care here will save you a lot of time later, because, when your system has a bug, the set of things that you have to check is much smaller.

<img src="images/badgood.jpg" width="350">

---
Here is a photo of Pat's completed breadboard. What a masterpiece! (Note the second blue button is optional and only used in the assignment extension.)
![Wired breadboard with components](images/wire1.jpg)

## Check in

Be sure to [check in](checkin) with a TA during lab. We want to confirm that you leave lab with a solid start on your clock breadboard and a clear understanding of how to complete the remaining tasks on your own.

__Please take care to return all tools to the shelves & leave your workspace as clean as you found it!__

