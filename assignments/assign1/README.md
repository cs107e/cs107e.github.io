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

[Glen Larson](http://en.wikipedia.org/wiki/Glen_A._Larson),
the producer of
the original
[Battlestar Galactica](http://en.wikipedia.org/wiki/Battlestar_Galactica)
and
[Knight Rider](http://en.wikipedia.org/wiki/Knight_Rider_%281982_TV_series%29)
television series,
often used simple lighting effects in his shows.
One famous example is a scanner that moves a light back and forth over a row of red LEDs.
The front of the Knight Rider car had a scanner.

This video tribute to Glen Larson shows how the pattern known as the "Larson scanner" got its name!

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

The assignment writeup details the requirements for the core features that everyone will implement. If you finish the core and are eager to go further, we encourage you to consider an extension. An extension is a chance to dig deeper into the assignment topics, pursue additional learning and experimentation, implement advanced features beyond the core, and/or be creative. We'll offer some suggestions for possible avenues of exploration and welcome your ideas as well. Is there is a topic you are particularly curious or passionate about? Pursuing an extension can help scratch that itch! We encourage everyone to try at least some of the extensions; they're a great way to challenge yourself and deepen your understanding.

{% comment %}
> __YEAH = Your Early Assignment Help__  The YEAH session is the kickoff for the week's assignment. In this group office hour session, we can answer your questions, inspire you about the new things you'll be learning, and facilitate a group brainstorm on best practices for success. YEAH meets in Gates B02 on Thursdays. Look for a post on the Ed form announcing specifics of the YEAH schedule for this quarter. Everyone welcome!
{: .callout-success}
{% endcomment %}

## Core functionality: simple scan pattern

Your assignment is to write a RISC-V assembly program
that animates a light moving back and forth across a set of LEDs.

### 1. Get starter files

Change to your local `mycode` repo and pull in the assignment starter code:

```console
$ cd ~/cs107e_home/mycode               # change to local mycode repo
$ git checkout dev                      # be sure on dev branch
$ git pull code-mirror assign1-starter  # pull starter code for this assign
```

Change to the new subdirectory `assign1` and list the starter files:

```console
$ cd assign1
$ ls
Makefile    README.md     extension.s    larson.s    style_reflection.txt
```
The starter files for `assign1` are:
- `larson.s`: an assembly language program. Edit this file to implement your Larson scanner
- `Makefile`: This file contains the build steps and is read by the `make` tool. You should not edit this file.
- `style_reflection.txt`: Edit this file with your responses to the style reflection prompts
- `README.md`: Edit this file to communicate with the grader about your submission
- `extension.s`: Edit this file to implement extension (if you choose)

### 2. Wire up your breadboard, connect to Pi

On your breadboard, make a row of at least four and no more than eight LEDS.

We hope to inspire you to share our appreciation for breadboarding as art and craftsmanship. Aim to make a tidy and pleasing circuit. Seat each LED and resistor close to the breadboard, snipping the ends to fit cleanly and using pliers to make neat bends. Not only does this look nice, your circuit will be easier to trace and debug. Making your connections more secure also means it will be more robust against mishaps from handling or being knocked around in your kit box.

Using the Mango Pi [pinout](/guides/refcard), connect a male-to-female jumper from PB0 to the leftmost LED, PB1 to the neighboring LED, and so on sequentially. Pro tip: `pinout.py PB0-7` for a road map of where to find the PB pins. Validate your wiring and components by temporarily connecting each LED to power to see that it lights up.

Show off your creation by snapping a photo of your finished breadboard/Pi and copy the file into the `assign1` directory. Use `git add` and commit the file to include it with your submission.

### 4. Confirm single blink

The code in the starter file `larson.s` is a copy of the blink program you studied in lab 1. It configures pin PB0 as an output and enters a loop to turn the pin on and off.

The starter project includes a `Makefile` that controls the build steps. Skim the Makefile now and review the comments to get the general gist. `make run` will build the larson program and execute on the Pi. If you `make run` on the unchanged starter program, it should blink the leftmost LED of your scanner (PB0).


```console
$ make run
riscv64-unknown-elf-as larson.s -o larson.o
riscv64-unknown-elf-objcopy larson.elf -O binary larson.bin
mango-run larson.bin
```

(If you're curious about `make` and makefiles, sit tight. We have an exercise on makefiles coming up in the next lab).


### 5. Configure all GPIOs used by scanner

Your task is to edit the program in `larson.s` to blink the scanner sequence instead of just PB0. Each of the GPIOs in the scanner must be configured to output mode. Recall that a GPIO's function is configured by modifying the associated bits within the `CFG` register. Refer to [Section 9.7 GPIO of the D1-H User Manual](/readings/d1-h_user_manual_v1.0.pdf#page=1083) for deets on the offsets and layout of the GPIO peripheral registers.

After editing the program to configure all the GPIOs as output, add some test code to confirm the configuration is correct. Change the loop to blink PB1,PB2,etc. instead of (or in addition to) PB0.

> __Commit yourself to regular commit__ Our strong recommendation is to start building the habit of making regular git commits. A commit takes a snapshot of your changes and records them into your repository history. This means you will be able to later review or revert to this  commit. When should you commit? Any time you hit an "interesting" point in your development: after achieving a milestone, before stopping for break, after fixing a bug, before embarking on an exploratory path, and so on. Be sure to include a descriptive message with each commit. Following up a commit with `git push` will sync your local repository to your GitHub remote repository, which additional gives you an "offsite" backup of your work.
{: .callout-success}

### 6. Generate scanner pattern

Now you are ready to implement the scanner pattern. The scanner steps from leftmost LED to rightmost and back again, lighting one LED at a time so the light appears to be "moving" between LEDs. Take care to handle the ends properly; the leftmost and rightmost LED should be on for the same amount of time as a middle LED. The scanning frequency should be around 1Hz (that is, scanning from one end to the other takes about 1 second).

Here is a video clip of an 8-LED scanner:

<video controls="controls" width="400"
    name="Larson scanner demo" src="images/larson_demo.mp4"></video>


Keep the [one-page guide](/guides/riscv-onepage) on hand as a reference. You may use any of the RISC-V instructions other than function calls, which we have yet to cover, i.e. do __not use__ `jal`,`jalr`,`call`, or `ret`.

Writing a bare-metal assembly program can be a bit of a power trip. Your program is king -- it is the only code executing and has unfettered access to the machine. You can use any and all of the registers, you have access to all of DRAM memory, you specify exactly which instructions to execute and how control flows. But with great power comes with great responsibility; it is up to you to track how you use the resources and not step on your own toes.

There are various ways you could structure your code, some more pleasing than others. We want your priority to be on writing clean, straightforward code that is easy for the reader to follow and understand. But even the cleanest of assembly doesn't serve as a standalone resource, be sure to include __detailed comments__ to explain yourself to the reader. Consider what information you would be grateful to have written down if you were to come back to this code a year from now and needed to quickly re-orient yourself and make a modification.

Turn down the lights, sit back and enjoy the show -- you have completed your first step on the path to bare-metal mastery!

## Style reflection

Each assignment has a companion "style reflection", a guided activity that directs you to make assessments about your code and allows you to share your progress in achieving your quality goals and your plans for how you can continue to improve. The style reflection will typically consist of a short reading and a few prompt questions. Your efforts to meaningfully engage with these materials and answer the prompts with thoughtful consideration contributes to your grade for assignment quality.

Here are the instructions for the [style reflection](style_reflection) to be submitted with Assignment 1.

## Extension

If you enjoyed the core assignment and want to explore further, consider tackling an extension. Below we offer a few possible ideas to choose from, but you should also consider blazing your own trail. What are you eager to learn more about?
- rework the base program into an upgraded form with superior readability/extensibility
    - use named assembly constants for number of LEDs and scanning frequency so they can be easily edited
    - learn ahead about how functions are implemented in assembly and decompose control flow into functions (simplest if have only one level, operation of nested functions is trickier)
- add button(s) that allow user to interactively control the scanner
    - pause while held down? increase and decrease speed? change pattern?
- explore controlling LED brightness using software pwm ("pulse width modulation")
    - the idea is to repeatedly switch an LED on and off at high enough rate that it is smoothed by the human eye into seeing it as "partially-on"
        - __software__ pwm manually sets gpio high and low and uses loops to control ratio of on/off time (hardware pwm using the D1-H `pwm` peripheral is a much more challenging task, not recommended to tackle as a first extension 😅)
    - apply an up-down ramp in brightness to simulate LED "breathing" or implement the classic Larson scanner that moves a group of five LEDs, where middle LED is full-on, immediate neighbors partially-on and outer neighbors even more dim. The effect can look really cool, see this [video from Evil Mad Science](https://www.youtube.com/watch?v=alW5zHiF6_0).
- design your own repeating light-show pattern
- random/chaotic
    - hmmm... how to simulate "randomness" from deterministic execution? Try search "pseudorandom number generator" to learn more.
- use the 8 LEDs as binary counter and display count of button clicks or elapsed seconds
    - read ahead on RISC-V Control and Status Registers (CSRs) to learn how to obtain time
- reaction timer game
    - one-way scan at variable speed, user has to click before reaches other end
- OR something else entirely, come by office hours to brainstorm your ideas with us!


Before diving into the extension, have your completed core program in `larson.s` and commit, tag, and push. This ensures your `assign1-submit` tag is a known good place for grading the core features. You also can nab the on-time reward by making core submit on or before due date.

Edit file `extension.s` with your extension program and use `make extension` to compile and run. Use the same git workflow as core, i.e. edit on dev branch, regularly commit and push. Apply tag `assign1-extension` to submit your finished extension.

Be sure to include helpful comments in your code to explain how it operates and edit the `README.md` file to tell your grader about what you implemented.

The same guidelines apply to the extension as to the core: __it is fine to use online resources and generative AI tools to answer your questions and help you learn new things, but not cool to use it to write or fix your code.__

## Submitting
The starter files for each assignment include an empty `README.md` file for communicating with the grader. Edit this file to pass along whatever information you'd like:
'I implemented extended feature X' or 'There is a lurking bug in function Y that I need help to resolve' or 'I am really proud of this neat thing Z I did in my code'. The file may also be left blank.

> __Note__ Make sure that you also cite any help that you receive, either in the `README.md` or as comments in your code, as explained in the [collaboration policy](/policies/#collaboration-policy).
{: .callout-warning}

The assignment is due at __5pm on Tuesday.__ The timestamp on the tagged commit determines the submission time. Delivering on schedule earns a small on-time reward. Learning how to pace your work to hit a deadline is a great skill to build! Finishing on time also allows you a short breather before the next assignment is released. If you are running a a bit behind in a given week, there is a 48-hour grace period where we accept late submissions without penalty. Read more about the [late policy](/policies/#late-policy).

The deliverables for the core `assign1-submit`:

- photo of your completed breadboard
- assembly program `larson.s`
- `README.md` with any information you want to share with your grader
- your responses to the prompt questions in `style_reflection.txt`
- all `assign1-submit` files commit/tag/push by due date or before end of grace period

Be sure to add and commit all files. Submit by tag `assign1-submit` on your final commit and push to remote. Follow the steps in submit checklist of [Git Workflow Guide](/guides/cs107e-git#assignment-checklist).

If you did an extension, submit `assign1-extension`:

- assembly program `extension.s`
- `README.md` with information on your extension
- use tag `assign1-extension`, commit/tag/push files by end of grace period

If core submit was made on-time, using the grace period for an extension does not forfeit its timeliness bonus (i.e. timeliness reward applies only to the core submit, not the extension).


## Grading

We evaluate __functionality__ and __quality__ of your submission. For functionality, we manually run the code and/or use automated tests to confirm correctness of required features. For qualitative review, the grader evaluates readability of code, thoughtfulness of style reflection, and other expectations per specifications in the assignment writeup. (General info on [assignment grading](/assignments/#faq))

Functionality evaluation:
- We will test on a Pi with LEDs connected to the PB0-7 pins
- Verify that `make` successfully builds your program with no warnings or errors.
- `make run` to execute larson scanner program and visually confirm its operation
    - 4-8 LEDs that individually light in correct pattern
    - Scanning pattern is even and smooth
    - Scans in both forward and reverse direction, bounce back at ends
    - Change of direction is handled correctly (no duplicate or skipped LEDs)

Qualitative review:
- Admire the photo of your breadboard that you submitted
- Review the readability of your assembly code
- Confirm the thoughtfulness of your completed style reflection

Extension demo/interview:
- Extensions will be reviewed in an interactive grading session. Look for a announcement from us about scheduled grading hours where you are invited to come in to show off your extension and chat about what you did and what you learned from it.
