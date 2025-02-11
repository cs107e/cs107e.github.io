---
released: true
permalink: /assignments/assign1/
title: "Assignment 1: Larson Scanner"
attribution: Assignment written by Pat Hanrahan
toc: true
---

{% comment %}
Task list to copy/paste when creating PR:

__Before releasing assign1:__

- [ ] Review writeup/starter code (instructor)
- [ ] Review consistency/completeness of grading info published to students relative to grading tests used, consider changes clarity/ease of grading (TA)
- [ ] Followup on issues from previous quarter postmortem (issue #467)

__To prep for assign1:__

- [ ]

{% endcomment %}

![Knight Rider Car](images/knightrider.jpg){: style="float:right;padding-left:10px"}

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

When you're done with this assignment, you will have practiced with

- reading and writing simple programs in RISC-V assembly,
- using the RISC-V cross-development tools,
- constructing simple LED circuits on a breadboard,
- and programming general-purpose input/output (GPIO) pins on the Mango Pi.

## How do assignments work?

The course follows a weekly cadence where a topic is introduced in the Friday-Monday lectures and the Tuesday-Wednesday lab follows up with hands-on guided practice. You leave lab ready to tackle the assignment and in the process of completing it, you solidify your understanding of the topic.

Each assignment has a required set of core features and optional extended features. Completing the core functionality is required for all students; the extensions are the optional "stretch goals" for those students aiming to go further. We award bonus credits for the extended features. We encourage everyone to try the extensions; they're a great way to challenge yourself and deepen your understanding.

> __YEAH = Your Early Assignment Help__  The YEAH session is the kickoff for the week's assignment. In this group office hour session, we can answer your questions, inspire you about the new things you'll be learning, and facilitate a group brainstorm on best practices for success. The YEAH session meets in Gates B02, usually on Thursday afternoon. See the Ed forum for post announcing YEAH schedule for this quarter. Everyone welcome!
{: .callout-success}


## Core functionality: simple scan pattern

Your assignment is to construct a breadboard circuit with a group of LEDs and
write a RISC-V assembly program
that animates a moving light back and forth across the group.

### 1. Get starter files

Change to your local `mycode` repo and pull in the assignment starter code:

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull code-mirror assign1-starter
```

Your repo should now be on the `dev` branch and should contain a new subdirectory named `assign1` containing the starter files. Change to that subdirectory and look around:

```console
$ cd assign1
$ ls
Makefile    README.md     extension.s    larson.s    style_reflection.txt
```
The files in the `assign1` directory are:
- `larson.s`: an assembly language program.  This is the file you will edit to implement your Larson scanner. The starter code is the blink program used in lecture/lab.
- `extension.s`: for the extension program if you attempt it.
- `README.md`:  a text file you can edit to communicate with the grader about your submission
- `Makefile`: this file contains the rules for how to build the program. This file is read by the `make` tool.
- `style_reflection.txt`: a text file that you will edit for your responses to the style reflection prompts

### 2. Wire up your breadboard, connect to Pi

The scanner is a row of at least 4 LEDS. You can connect 6 or 8 LEDS if you'd like to see a fancier display.

Take the time to do a nice job building your breadboard! We want you to share our appreciation for breadboarding as art and craftsmanship. Secure your components close to the breadboard, snipping the ends to fit cleanly and using your pliers to make neat bends. Not only does this look nice, it makes your circuit easier to trace and debug. This will also make it more robust to being handled or knocked around in your kit box.

The leftmost LED in your scanner should be connected to gpio PB0 of your Mango Pi, the second from the left to PB1, and so on, such that the rightmost LED in your scanner is connected to PB3 (or PB7 if using 8 LEDs). Refer to your [refcard](/guides/refcard) to map each GPIO to the physical pin so that you correctly wire up the connections. Validate your breadboard wiring by testing that each LED lights up when powered.

Share your creation by snapping a photo of your larson scanner hardware setup and copying it to the `assign1` directory of your `mycode` repo. Use `git add` and commit the file so that it is included with your submission.

### 4. Confirm single blink

The code in the starter file `larson.s` is a copy of the blink program you studied in lab 1. This program configures the single pin PB0 as an output and enters an infinite loop to turn that pin on and off. Carefully review this code and be sure you understand how it accomplishes its tasks. Ask questions if anything is unclear!

Build and run the starter program with your breadboard connected. The starter program should blink the leftmost LED of your scanner (PB0). The `make run` command will build the program and send it to the bootloader to run on the Pi shown below:

```console
$ make run
riscv64-unknown-elf-as larson.s -o larson.o
riscv64-unknown-elf-objcopy larson.o -O binary larson.bin
mango-run larson.bin
```

If you're wondering how `make` and makefiles work, sit tight. We have an lab exercise on makefiles coming up lab2.

### 5. Configure all scanner GPIOs

You will modify the program in `larson.s` to blink the scanner sequence, rather than the single LED.

A good first task is to configure as output all of the GPIOs used by your scanner, instead of just PB0. Recall that a GPIO's function is selected by modifying the associated bits in the `CFG` register for that pin index of the PB group. Refer to section 9.7 of the [D1 User Manual](/readings/d1-h_user_manual_v1.0.pdf#page=1083) for the details on the GPIO peripheral registers.

After writing your configuration code, add some test code to verify that this code works correctly. You can test an individual pin by changing the existing loop to blink PB1 or PB2 and so on instead of PB0. Once you have verified that all pins are properly configured, you're now ready to move on to blinking the pins in the scanner sequence.

> __Commit yourself to regular commits__ Our strong recommendation is that you make a habit of regular git commits. A commit takes a snapshot of your changes and records them into your repository history. This takes a snapshot that allows you to later return to this version if need be. When should you commit? Any time you hit an "interesting" point in your development: after achieving a milestone, before stopping for break, after fixing a bug, before embarking on an exploratory path, and so on. Be sure to include a descriptive message with each commit. Following up a commit with `git push` will sync your local repository to your GitHub remote repository.
{: .callout-success}

### 6. Generate scanner pattern

The scanner loop should scan back and forth from the leftmost to rightmost LED. Each iteration of the loop still turns on/off one GPIO, but instead of blinking the same GPIO each time, it will blink the next GPIO pin in sequence.

The scanning pattern should move smoothly back and forth. In particular, exactly one LED should be on at any given time, so the light appears to be "moving" between LEDs. Make sure you handle the left and right
ends properly (that is, the leftmost and rightmost LED should be each
on for the same amount of time that a middle LED would be on). The scanning frequency should be around 1Hz (that is, the light should bounce back and force about once per second).

Here is a video clip of an 8-LED scanner:

<video controls="controls" width="400"
       name="Assignment 1 demo" src="images/larson_demo.mp4"></video>


Keep the [one-page guide](/guides/riscv-onepage) on hand as your reference to all RISC-V instructions. You are free to use the full range of instructions other than function calls, which we have not yet covered, i.e. do not use `jal`/`jalr`/`call`/`ret`.

There are many ways to structure the code, some more pleasing than others.  We want your priority to be on writing clean, straightforward code that is easy for the reader to follow and understand. A much lesser side goal is to support modest extensibiity, such as making it easy to edit the number of leds from 4 to 6 or change the speed.

Writing readable assembly is a challenging task; __detailed comments are essential__.  Consider what information you would be grateful to have written down if you were to come back to this code a year from now and needed to quickly re-orient yourself and make a modification.

Turn down the lights, sit back and enjoy the show -- you have completed your first step on the path to bare-metal mastery!

## Style reflection

Each assignment has a companion "style reflection", a guided activity that directs you to make assessments about your code and allows you to share your progress in achieving your quality goals and your plans for how you can continue to improve.  The style reflection will typically consist of a short reading and a few prompt questions. Your efforts to meaningfully engage with these materials and answer the prompts with thoughtful consideration contributes to your code quality grade.

Here are the instructions for the [style reflection](style_reflection) to be submitted with Assignment 1.

## Submitting
The starter files for each assignment include an empty `README.md` file for communicating with the grader. Edit this file to pass along whatever information you'd like:
'I implemented extended feature X' or 'There is a lurking bug in function Y that I need help to resolve' or 'I am really proud of this neat thing Z I did in my code'. The file may also be left blank.

> __Note__ Make sure that you also cite any help that you receive, either in the `README.md` or as comments in your code, as explained in the [collaboration policy](/policies/#collaboration-policy).
{: .callout-warning}

The submit checklist in the [Git Workflow Guide](/guides/cs107e-git#assignment-checklist) has the final steps: __tag__ your
submit and __push__ to sync with your remote repo.

The assignment is due at __5pm on Tuesday.__ The timestamp on the tagged commit determines the submission time.  Delivering on schedule earns a small on-time reward. We encourage you to learn how to pace your work and hit the deadline -- this is a great skill to build! Finishing on time also allows you a short breather before the next assignment comes out. If you are running a a bit behind in a given week, there is a 48-hour grace period where we accept late submissions without penalty (although you miss out on the on-time reward). Read more about the [late policy](/policies/#late-policy).

## Extension

If you enjoyed the core assignment portion and want to explore further, we encourage you to tackle the extension. The extension is more challenging, and will involve further explorations of RISC-V assembly and writing more complicated code. Completing this extension is worth __2 extension bonus credits__.

{% comment %}
 Think of how you might create a much more simplistic version of PWM. Specifically, "brightness" is created by making an LED switch very quickly between on and off such that the percentage of on -vs- off time gives you an apparent brightness. For example, let's say you want 50% brightness. Then, the total time for on -vs- off should be equal during a given period. But, the tricky part is that you can't simply say that in 1 second, you turn on the LED for 0.5 sec, and then off for 0.5 sec -- this just looks like a blinking light. To fool your eye into seeing "brightness", you need to turn the LED on and off very frequently during that 1 second interval -- thousands of times would be a good strategy. Your eye won't see the individual blinking (it will be too fast), and you will interpret the pattern as brightness.
{% endcomment %}

If you watch the Larson scanner carefully,
several LEDs are turned on at once.
The center one is at full brightness,
and the ones on the edge are dimmer. How can you dim an LED?
You may remember from lab how to change the LED brightness with a hardware tweak such as applying a smaller resistor or supplying a different voltage. That won't work here-- the pins on the Mango Pi are digital and have only two settings: on (3.3V) or off (0), nothing in between. However, it is possible in software to very quickly switch the LED on and off in such a way to simulate an effect that the human eye will perceive as "semi-on". Neat!

Your challenge for the assignment extension is to
implement a scanner with LEDs aesthetically dimmed
in a way that would please Glen Larson.

The extension requires 8 LEDs. There is a full-brightness LED in the center scans back and back just as
in the core version, but you should visibly dim the
neighbors of that central light as it moves. Aim for a look similar to
the
[Larson Scanner Kit Demo](https://www.youtube.com/watch?v=yYawDGDsmjk).

You should have at least three distinct levels of brightness (not including "zero brightness," or the lights that are off).

This fancy code will surely require thorough comments to explain how it works!

{% comment %}
FOR NEXT TIME: require that larson start at standstill with 3 levels of brightness for 5 seconds, and only then start moving
{% endcomment %}

The extension program is to be written in the file `extension.s`. Copy your completed code from `larson.s` into `extension.s` as your starting point. Use `make extension` to compile and run your extension program.

Before diving into the extension, be sure you have completed all of your basic scanner, including commit, tag, and push. This ensures your submission tag is a known good place that can be a fallback if the extension doesn't pan out.

When working on the extension, follow the same git workflow as core (edit on dev branch, make regular commits and pushes). When you have successfully completed the extension, tag your final commit with `assign1-extension` and push to indicate that your extension is ready to be graded.

## Grading

In grading, we evaluate the __functionality__ and __quality__ of your submission. For functionality, we check for completeness and correctness by manually running the code and/or automated tests to confirm each required feature. For qualitative review, the grading CA evaluates the issues as per the particular instructions in assignment writeup (e.g. readability of code, comprehensiveness of required test cases, thoughtfulness of style reflection, etc.)

To test your Larson scanner, the grading CA will invoke `make` to build your `larson.bin`
file and execute on a Pi with the LEDs connected to the PB group pins. Be sure that:

- A `make` will successfully build your program with no warnings or errors.
- Your scanner uses pins PB0-PB3 or PB0-PB7 (depending on how many LEDs you chose to use).

Our functionality tests will confirm the following criteria are met:

- 4-8 LEDs that individually light in correct pattern
- Scanning pattern is even and smooth
- Scans in both forward and reverse direction
- Change of direction is handled correctly (no duplicate or skipped LEDs)
- For extension: at least three differentiated levels of brightness, scanning has pleasing ramp and smoothness

Our qualitative review will admire the readability of your assembly code and confirm the thoughtfulness of your completed style relection.

For more long-winded information on grading and course policy, see our pages on [assignments](/assignments/#faq) and [course policies](/policies).
