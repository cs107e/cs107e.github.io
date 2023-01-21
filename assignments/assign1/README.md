---
released: true
permalink: /assignments/assign1/
title: "Assignment 1: Implement a Larson Scanner"
toc: true
---

{% comment %}
Task list to copy/paste when creating PR:

**Before releasing assign1:**

- [ ] Review writeup/starter code (instructor)
- [ ] Review consistency/completeness of grading info published to students relative to grading tests used, consider changes clarity/ease of grading (TA)
- [ ] Followup on issues from previous quarter postmortem (issue #467)

**To prep for assign1:**

- [ ]

{% endcomment %}

![Knight Rider Car](images/knightrider.jpg){: style="float:right;"}

_Written by Pat Hanrahan_

## {% include duedate.html n=1 %}

[Glen A. Larson](http://en.wikipedia.org/wiki/Glen_A._Larson),
the man responsible for producing
the original
[Battlestar Galactica](http://en.wikipedia.org/wiki/Battlestar_Galactica)
and
[Knight Rider](http://en.wikipedia.org/wiki/Knight_Rider_%281982_TV_series%29)
television series,
often used simple lighting effects in his shows.
One famous example is a scanner
that consists of a set of red LEDs that scan back and forth.
The front of the Knight Rider car had a scanner.

Watch this video tribute to Glen Larson.
How many times does the scanner pattern appear?

<iframe width="500" height="281" src="https://www.youtube-nocookie.com/embed/hDUoyeMLxqM?modestbranding=1" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

---

The Larson scanner demoed in this video is a [DIY kit from Evil Mad Scientist Labs](https://shop.evilmadscientist.com/productsmenu/tinykitlist/152-scanner).

<iframe width="350" height="196" src="https://www.youtube-nocookie.com/embed/alW5zHiF6_0?modestbranding=1&end=10&version=3&loop=1&playlist=alW5zHiF6_0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## Goals

When you're done with this assignment, you should

- understand ARM assembly language,
- feel fluent with the ARM cross-development tools,
- know how to use breadboards and use LEDs,
- and be able to program general-purpose input/output (GPIO) pins.

## How do assignments work?

The course follows a weekly cadence where material is introduced in the Friday-Monday lectures and the Tuesday-Wednesday lab follows up with hands-on guided practice so that you're ready to tackle the assignment (released after Wed lab). Completing the assignment will solidify your understanding of the topics. In additiona, each of Assignments 2 through 7 contributes a building block to your library of Pi features. Completing this system is an accomplishment of which you will be immensely proud!

Each assignment is structured into the core requirements and one or more proposed extensions. Completing the core functionality is required for all students; the extensions are the optional "stretch goals" for those students aiming to go further. We encourage everyone to try the extensions; they're a great way to challenge yourself and solidify your understanding.

> **YEAH = Your Early Assignment Help**  Each Thursday we'll hold a YEAH session as a kickoff for the week's assignment and more quality time with the CS107e family! We can answer your questions, pump you up for the neat things you'll be learning, and facilitate a group brainstorm on practices for success. YEAH Hours for Assignment 1 will take place Thursday 8-9pm in B02. Everyone welcome!
{: .callout-success}

Please refer to the [Grading and Late Policies](/policies) for detailed information about the grading policy. Additionally, you can take a look at [Assignments FAQ](/assignments/#faq) for more information about how your assignments are graded. 

## Core functionality: simple scan pattern

This assignment involves modifying the blink program showed in class.
`blink.s` is an ARM assembly language program that blinks a single
LED. You will extend the program to control four LEDs instead of just one.
Using arithmetic and loops, your assembly program changes which
LED is lit to show a scanner pattern.

### 1. Get starter files

Before starting each new assignment, first do a `git pull` in your $CS107E repository to ensure the courseware files are up to date.

```console
$ cd $CS107E
$ git pull
```

Now cd to your local `mycode` repo and pull in the assignment starter code:

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull --allow-unrelated-histories starter-code assign1-starter
```

Your repo should now be on the `dev` branch and should contain a new subdirectory named `assign1` containing the starter files. Change to that subdirectory and look around:

```console
$ cd assign1
$ ls
Makefile      README.md    larson.s
```
There are three files in the `assign1` directory:
- `larson.s`: an assembly language program.  This is the file you will edit to implement your Larson scanner.
- `README.md`:  a text file you can edit to communicate with the grader about your submission
- `Makefile`: this file contains the rules for how to build the larson program. This file is read by the `make` tool.


### 2. Wire up your breadboard

The scanner should have a row of 4 LEDs. If you feel ambitious and want to make more interesting patterns, wire up 8 LEDs. Validate your breadboard wiring by testing that each LED lights up when powered.

Take the time to do a good job! We want you to share our appreciation for breadboarding as art and craftsmanship. Secure your components close to the breadboard, snipping the ends to fit cleanly and using your pliers to make neat bends. Not only does this look nice, it will be more robust to being handled.

### 3. Connect the LEDs to GPIO 20-23

The leftmost LED in your scanner should be connected to GPIO 20 of your Raspberry Pi, the second from the left to GPIO 21, and so on, such that the rightmost LED in your scanner is connected to GPIO 23 (or GPIO 27 if using 8 LEDs). Refer to the diagram of the [pinout](/guides/images/pinout.pdf) to map each GPIO to the physical pin so that you correctly wire up the connections.

Show off your creation by snapping a photo of your larson scanner hardware setup and copying it to the `assign1` directory of your `mycode` repo. Don't forget to `git add` and commit the file so that it is included with your submission!

### 4. Confirm single blink

The code in the starter file `larson.s` is a copy of the blink program you studied in lab 1. This program configures the single pin GPIO 20 as an output and enters an infinite loop to set and clear that pin. Carefully review this code and be sure you understand how it accomplishes its tasks. Ask questions if anything is unclear!

Build and run the starter program on your hardware setup. The starter program should blink the leftmost LED of your scanner (GPIO 20). The `make run` command will build the program and send it to the bootloader to run on the Pi shown below:

```console
$ make run
arm-none-eabi-as larson.s -o larson.o
arm-none-eabi-objcopy larson.o -O binary larson.bin
rpi-run.py larson.bin
```

If you're wondering how `make` and makefiles work, sit tight. We have an lab exercise on makefiles coming up lab2.

### 5. Configure all scanner GPIOs

You will modify the program in `larson.s` to blink the scanner sequence, rather than the single GPIO 20.

A good first task is to extend the initialization code to apply to not just GPIO 20, but all of the GPIOs used by your scanner. All of these GPIOs should be configured as output pins. Recall that a GPIO's function is selected by modifying the bits in the `FSEL` register associated with this GPIO. Refer to section 6 of the [Broadcom ARM Peripheral](/readings/BCM2835-ARM-Peripherals.pdf) for the details on `FSEL` and other GPIO registers.

After writing your configuration code, run tests to verify that this code works correctly. You can test an individual pin by changing the existing loop to blink GPIO 21 or GPIO 22 and so on instead of GPIO 20. Once you have verified that all pins are properly configured, you're now ready to tackle changing the loop body to blink the pins in the scanner sequence.

> __Commit yourself to regular commits__ Our strong recommendation is that you make a habit of regular git commits. A commit takes a snapshot of your changes and records them into your repository history. This takes a snapshot that allows you to later return to this version if need be. When should you commit? Any time you hit an "interesting" point in your development: after achieving a milestone, before stopping for break, after fixing a bug, before embarking on an exploratory path, and so on. Be sure to include a descriptive message with each commit. Following up a commit with `git push` will sync your local repository to your GitHub remote repository.
{: .callout-success}

### 6. Generate scanner pattern

The scanner loop should scan back and forth from the leftmost to rightmost LED. Each iteration of the loop still turns on/off one GPIO, but instead of blinking the same GPIO each time, it will blink the next GPIO pin in sequence.

The scanning pattern should move smoothly back and forth. In particular, exactly one LED should be on at any given time, so the light appears to be "moving" between LEDs. Make sure you handle the left and right
ends properly (that is, the leftmost and rightmost LED should be each
on for the same amount of time that a middle LED would be on). The scanning frequency should be around 1Hz (that is, the light should bounce back and force about once per second).

Here is a short video of our 8-LED scanner in action:

<video controls="controls" width="400"
       name="Assignment 1 demo" src="images/larson_demo.mp4"></video>

 Writing clean, readable assembly code is a challenge. Commenting is essential!  
Be sure to choose good label names and add named constants using `.equ` to aid readability.
Because you don't know how to write functions yet in assembly, you'll have to
write your program without the aid of this useful abstraction.  You otherwise have the full range of ARM assembly at your disposal; keep our [one-page guide to ARM](/guides/arm) on hand as a quick reference.

A branch instruction is the assembly equivalent of a goto (a practice long considered harmful <https://en.wikipedia.org/wiki/Considered_harmful>), and code with lots of branches can be hard to debug and follow.
Straight-line code is code without branches and is almost always easier
to understand. However, a downside of a straight-line approach is that you may end up with duplicated code. There is a tension here between less code and clearer code. For this first assignment, prioritize writing code that can be easily understood, whether it's by you six months from now
or by us in a week when we grade your assignment.

Congratulations on completing your first step on the path to bare-metal mastery!
Sit back and enjoy the show by turning off the lights and impressing your
friends.

## Submitting
The starter files for each assignment include an empty `README.md` file for communicating with the grader. Edit this file to pass along whatever information you'd like:
'I was not able to complete feature X' or 'There is a lurking bug in function Y that I need help to resolve' or 'I am really proud of this neat thing I did in my code'. The file may also be left blank.

The submit checklist in the [Git Workflow Guide](/guides/cs107e-git#assignment-checklist) has the final steps: __tag__ your
submit, __push__ to sync with your remote repo, and ensure you have an open __pull request__ on GitHub.

The assignment is due at 5pm on Tuesday. The timestamp on the tagged commit determines the submission time.  Delivering on schedule earns a small on-time bonus. We encourage you to learn how to pace your work and hit the deadline -- this is a great skill to build! Finishing on time also allows you a short breather before the next assignment comes out. If you are running a a bit behind in a given week, there is a 48-hour grace period where we accept late submissions without penalty. Read more about the [late policy](/policies/#late-policy).

## Extension

If you enjoyed the core assignment portion and want to explore further, we encourage you to tackle the extension. The extension is more challenging, and may involve using additional
ARM assembly instructions. It will almost certainly require more complicated code
involving conditional branches.

Your extension will build on your core implementation in the same `larson.s` file. As such, we'll expect that your extension implementation works as well, or better, than your core implementation.

If you watch the Larson scanner carefully,
several LEDs are turned on at once.
The center one is at full brightness,
and the ones on the edge are dimmer.
How would you dim an LED? Your challenge for the assignment extension is to
implement a scanner with LEDs aesthetically dimmed
in a way that would please Glen Larson.

The extension should have a central, full-brightness led that scans back and back just as
in the core version, but you should visibly dim the
neighbors of that central light as it moves. Aim for a look similar to
the
[Larson Scanner Kit Demo](https://www.youtube.com/watch?v=yYawDGDsmjk).

You should have at least three distinct levels of brightness.

Before starting on the extension, be sure you have completed all core tasks, including commit, tag, and push of your finished scanner. This ensures your submission tag is a known good place that can be a fallback if the extension doesn't pan out. 

When working on the extension, follow the same git workflow as core (edit on dev branch, regular commits and pushes). When you have successfully completed the extension, tag with `assign1-extension` and push to indicate that your extension is ready to be graded.

## Grading

Our general process for grading assignments focuses on evaluating the __functionality__ and __quality__ of your submission. For functionality, we assess the completeness and correctness of the implementation of required features. We run tests against the code for a variety of situations and report results to you in terms of passed and failed test cases. Quality comprises an evaluation of the design of your implementation, style and readability of your code, and thoroughness of your test cases.  We evaluate quality through an individual review and provide qualitative and constructive feedback.

To grade your Larson scanner, your beloved CAs will invoke `make` to build your `larson.bin`
file and test it on a Pi with the LEDs plugged in. For this process to go smoothly,
please ensure the following:

- A `make` will successfully build your program with no warnings or errors.
- You use pins 20-23 or 20-27 (depending on how many LEDs you chose to use).

While grading, we'll be checking for the following criteria to be met:

- 4-8 LEDs that individually light
- Scanning pattern is even and smooth
- Scans in both forward and reverse direction
- Reverse at end is handled correctly (no duplicate or skipped LEDs)
- (extension) At least three differentiated levels of brightness

For long-winded general information on grading and policy, see our pages on [assignments](/assignments/#faq) and [course policies](/policies).
