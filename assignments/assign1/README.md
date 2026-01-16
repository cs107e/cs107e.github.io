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
the producer of
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


## Goals

In completing this assignment, you will have practiced with

- reading and writing simple programs in RISC-V assembly,
- using the RISC-V cross-development tools,
- constructing simple LED circuits on a breadboard,
- and programming general-purpose input/output (GPIO) pins on the Mango Pi.

## How do assignments work?

The course follows a weekly cadence where a topic is introduced in the Friday-Monday lectures and the Tuesday-Wednesday lab follows up with hands-on guided practice. You leave lab ready to tackle the assignment and in the process of completing it, you solidify your understanding of the topic.

Each assignment is structured as a required set of core features with one or more optional extensions. Completing the core features is required for all students. The extensions are optional "stretch goals" to prompt you to go further. Extensions are rewarded with bonus credits. The extension tasks have less guidance and are more open-ended, inspiring you to pursue additional learning and experimentation and allow for creativity.
We encourage everyone to try at least some of the extensions; they're a great way to challenge yourself and deepen your understanding.
> __YEAH = Your Early Assignment Help__  The YEAH session is the kickoff for the week's assignment. In this group office hour session, we can answer your questions, inspire you about the new things you'll be learning, and facilitate a group brainstorm on best practices for success. YEAH meets in Gates B02 on Thursdays. Look for a post on the Ed form announcing specifics of the YEAH schedule for this quarter. Everyone welcome!
{: .callout-success}


## Core functionality: simple scan pattern

Your assignment is to write a RISC-V assembly program
that animates a light moving back and forth across a sequence of LEDs on a breadboard.

### 1. Get starter files

Change to your local `mycode` repo and pull in the assignment starter code:

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull code-mirror assign1-starter
```

Your repo is now on the `dev` branch and should contain a new subdirectory named `assign1`. Change to that subdirectory and list the starter files:

```console
$ cd assign1
$ ls
Makefile    README.md     extension.s    larson.s    style_reflection.txt
```
The starter files for `assign1` are:
- `larson.s`: an assembly language program.  Edit this file to implement your Larson scanner
- `Makefile`: This file contains the build steps and is read by the `make` tool. You should not edit this file.
- `style_reflection.txt`: Edit this file with your responses to the style reflection prompts
- `README.md`: Edit this file to communicate with the grader about your submission
- `extension.s`: Edit this file to implement the extension program (if you attempt it)

### 2. Wire up your breadboard, connect to Pi

Make a row of at least four LEDS on your breadboard. You can connect up to eight LEDS to make a fancier display.

We hope to inspire you to share our appreciation for breadboarding as art and craftsmanship! Aim to make a tidy and pleasing circuit. Seat each LED and resistor close to the breadboard, snipping the ends to fit cleanly and using pliers to make neat bends. Not only does this look nice, your circuit will be easier to trace and debug. Making your connections more secure also means it will be more robust against mishaps from handling or being knocked around in your kit box.

Connect the leftmost LED to PB0, the nextmost LED to PB1, and so on, such that the rightmost LED connects to PB3 (or PB7 if using 8 LEDs). Use your [pinout refcard](/guides/refcard) to identify where the PB pins are on the Mango Pi header. (Hot tip: Try command `pinout.py larson` for a road map) Validate your wiring and components by temporarily connecting each LED to power to see that it lights up.

Show off your creation to us by snapping a photo of your final breadboard/Pi and copying the file to the `assign1` directory of your `mycode` repo. Use `git add` and commit the file so that it is included with your submission.

### 4. Confirm single blink

The code in the starter file `larson.s` is a copy of the blink program you studied in lab 1. It configures pin PB0 as an output and enters a loop to turn the pin on and off.

The starter project includes a `Makefile` that controls the build steps. Skim the Makefile now and review the comments in the file to get the general gist.  `make run` is the command to build the larson program and execute on the Pi. If you `make run` on the starter program unchanged, it should blink the leftmost LED of your scanner (PB0).


```console
$ make run
riscv64-unknown-elf-as larson.s -o larson.o
riscv64-unknown-elf-objcopy larson.elf -O binary larson.bin
mango-run larson.bin
```

(If you're curious about `make` and makefiles, sit tight. We have an exercise on makefiles coming up in the next lab).


### 5. Configure all GPIOs in scanner

You will be modifying the program in `larson.s` to blink the scanner sequence instead of just PB0. Each of the GPIOs in the scanner must be configured to output mode. Recall that a GPIO's function is configured by modifying the associated bits within the `CFG` register. Refer to [Section 9.7 GPIO of the D1-H User Manual](/readings/d1-h_user_manual_v1.0.pdf#page=1083) for deets on the offsets and layout of the GPIO peripheral registers.

After editing the program to configure all the GPIOs as output, add some test code to confirm the configuration is correct. Change the loop to blink PB1,PB2,etc. instead of (or in addition to) PB0.

> __Commit yourself to regular commit__ Our strong recommendation is to start building the habit of making regular git commits. A commit takes a snapshot of your changes and records them into your repository history. This means you will be able to later review or revert to this  commit. When should you commit? Any time you hit an "interesting" point in your development: after achieving a milestone, before stopping for break, after fixing a bug, before embarking on an exploratory path, and so on. Be sure to include a descriptive message with each commit. Following up a commit with `git push` will sync your local repository to your GitHub remote repository, which additional gives you an "offsite" backup of your work.
{: .callout-success}

### 6. Generate scanner pattern

Now you are ready to implement the scanner pattern to step from leftmost LED to rightmost and back again. Exactly one LED should be on at any given time, so the light appears to be "moving" between LEDs. Take care to handle the ends properly -- the leftmost and rightmost LED should be on for the same amount of time as a middle LED. The scanning frequency should be around 1Hz (that is, the light should bounce back and forth about once per second).

Here is a video clip of an 8-LED scanner:

<video controls="controls" width="400"
       name="Assignment 1 demo" src="images/larson_demo.mp4"></video>


Keep the [one-page guide](/guides/riscv-onepage) on hand as a reference. You may use any of the RISC-V instructions other than function calls, which we have yet to cover, i.e. do __not use__ `jal`,`jalr`,`call`, or `ret`.

Writing a bare-metal assembly program is a bit of a power trip. Your program is king -- it is the only code executing and has unfettered access to the machine. You can use any and all of the registers, you have access to all of DRAM memory, you specify exactly which instructions to execute and how control flows.  But with great power comes with great responsibility; it is up to you to track how you use the resources and not step on your own toes.

There are various ways you could structure your code, some more pleasing than others.  We want your priority to be on writing clean, straightforward code that is easy for the reader to follow and understand. A much lesser side goal is to support modest extensibility, such as making it easy to edit the number of LEDs from 4 to 6 or change the speed.

Writing readable assembly is a challenging task; __detailed comments are essential__.  Consider what information you would be grateful to have written down if you were to come back to this code a year from now and needed to quickly re-orient yourself and make a modification.

Turn down the lights, sit back and enjoy the show -- you have completed your first step on the path to bare-metal mastery!

## Style reflection

Each assignment has a companion "style reflection", a guided activity that directs you to make assessments about your code and allows you to share your progress in achieving your quality goals and your plans for how you can continue to improve.  The style reflection will typically consist of a short reading and a few prompt questions. Your efforts to meaningfully engage with these materials and answer the prompts with thoughtful consideration contributes to your grade for assignment quality.

Here are the instructions for the [style reflection](style_reflection) to be submitted with Assignment 1.

## Extension

If you enjoyed the core assignment and are curious to explore further, we encourage you to tackle the extension. The extension is more challenging, and will involve further explorations of RISC-V assembly and implementing more complex code. Completing this extension is worth __2 extension bonus credits__. (Additional info on [grading policy](/policies))

If you carefully watch the Larson scanner in the video,
more than one LEDs is on at once.
The center one is at full brightness; the neighboring ones are progressively dimmer. How can you dim an LED?
In lab, you changed the LED brightness by applying a smaller resistor or supplying a different voltage. But a hardware approach won't work here-- the pins on the Mango Pi are __digital__ and in software, our only control is to either turn on (3.3V) or off (0), nothing in between. What we __can__ do in software is rapidly flip the LED on and off to simulate an effect that the human eye will perceive as "brightness".

 For example, to light an LED for one second at 50% brightness, you want the LED to be on for half of the total time and off for half. If you simply turned the LED on for half a second and then off for half a second, you would see one short blink. But if you instead flip the LED on and off a gazillon times during the entire interval, it will fool your eye into seeing "brightness". The blinks come way too fast to see them individually, so instead you perceive the LED as "semi-on".  Neat!

The tasks for the extension are to
implement a smooth-scanning pattern with LEDs aesthetically dimmed
in a way that would please Glen Larson and add a pause/resume button.

The extension requires 8 LEDs. There should be a full-brightness LED in the center scans back and back just as
in the core version and you should visibly dim the
neighbors of that central light as it moves. There should be at least three distinct levels of brightness (that is, three levels not counting the "zero brightness" LEDs that are off). Aim for a look similar to the video below (a [DIY kit from Evil Mad Scientist Labs](https://shop.evilmadscientist.com/productsmenu/tinykitlist/152-scanner)).

<iframe width="350" height="196" src="https://www.youtube-nocookie.com/embed/alW5zHiF6_0?modestbranding=1&end=10&version=3&loop=1&playlist=alW5zHiF6_0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

For pause/resume, add a push button to your breadboard circuit that is connected to PC0 through a pull-up resistor (just as in the [button exercise from lab1](/labs/lab1#button)). When the button is not pressed, the scanner moves normally. While the button is pressed, movement is paused. While paused, the LEDs that are currently on should stay lit at their graduated levels of brightness. Using the button to pause movement allows you to more easily observe the dimming of the neighboring LEDs around the center.


The extension program is to be written in the file `extension.s`. Copy your completed `larson.s` into `extension.s` as your starting point. Use `make extension` to compile and run your extension program.  Your fancy upgraded program will surely require thorough comments to explain how it works!

Before diving into the extension, be sure to complete core scanner, including commit, tag, and push. This ensures your submission tag is a known good place that can be a fallback if the extension doesn't pan out. You also can claim the timeliness bonus by commit/tag/push the core program by due date.

When working on the extension, follow the same git workflow as core (edit on dev branch, make regular commits and pushes). When finished, tag your final commit `assign1-extension` and push. This tag and push submits the extension for grading. Extension submissions must be received by the end of the grace period.  If the core submission was made on-time, using the grace period for the extension does not forfeit its timeliness bonus (i.e. timeliness bonus applies only to the core submission, not the extension).


## Submitting
The starter files for each assignment include an empty `README.md` file for communicating with the grader. Edit this file to pass along whatever information you'd like:
'I implemented extended feature X' or 'There is a lurking bug in function Y that I need help to resolve' or 'I am really proud of this neat thing Z I did in my code'. The file may also be left blank.

> __Note__ Make sure that you also cite any help that you receive, either in the `README.md` or as comments in your code, as explained in the [collaboration policy](/policies/#collaboration-policy).
{: .callout-warning}

The assignment is due at __5pm on Tuesday.__ The timestamp on the tagged commit determines the submission time.  Delivering on schedule earns a small on-time reward. Learning how to pace your work to hit a deadline is a great skill to build! Finishing on time also allows you a short breather before the next assignment is released. If you are running a a bit behind in a given week, there is a 48-hour grace period where we accept late submissions without penalty. Read more about the [late policy](/policies/#late-policy).

The deliverables for `assign1-submit` are:

- photo of your completed breadboard
- assembly program `larson.s`
- `README.md` with any information you want to share with your grader
- your responses to the prompt questions in `style_reflection.txt`

Be sure to add and commit all files. Submit by tag `assign1-submit` on your final commit and push to remote. Follow the steps in submit checklist of [Git Workflow Guide](/guides/cs107e-git#assignment-checklist).


## Grading

We evaluate __functionality__ and __quality__ of your submission. For functionality, we manually run the code and/or use automated tests to confirm correctness of required features. For qualitative review, the grader evaluates readability of code, thoughtfulness of style reflection, and other expectations per specifications in the assignment writeup. (General info on [assignment grading](/assignments/#faq))

Functionality evaluation:
 - We do our testing on a Pi with LEDs connected to the PB group pins and button connected to PC0 through pull-up resistor.
- Verify that `make` successfully builds your program with no warnings or errors.
- `make run` to execute larson scanner program on the Pi and visually confirm its operation
    - 4-8 LEDs that individually light in correct pattern
    - Scanning pattern is even and smooth
    - Scans in both forward and reverse direction
    - Change of direction is handled correctly (no duplicate or skipped LEDs)

Qualitative review:
- Admire the photo of your breadboard that you submitted
- Review the readability of your assembly code
- Confirm the thoughtfulness of your completed style reflection

For the extension, we expect:
- At least three differentiated levels of brightness
- Scanning has pleasing ramp and smoothness
- Press button pauses scanning movement, resume on release

