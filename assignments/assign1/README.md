---
released: true
permalink: /assignments/assign1/
title: 'Assignment 1: Implement a Larson Scanner'
toc: true
---
{% comment %}
Task list to copy/paste when creating PR:

__Before releasing assign1:__
- [ ] Review writeup/starter code (instructor)
- [ ] Review consistency/completeness of grading info published to students relative to grading tests used, consider changes clarity/ease of grading (TA)
- [ ] Followup on issues from previous quarter postmortem (issue #)

__To prep for assign1:__
- [ ] 

{% endcomment %}

![Knight Rider Car](images/knightrider.jpg){: style="float:right;"}

*Written by Pat Hanrahan*

{% include duedate.html n=1 %}
---
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
The course follows a weekly cadence where material is introduced in the Friday-Monday lectures and lab follows up with hands-on guided practice so that you're ready to tackle the week's assignment. The assignment is released after Wednesday lab and due before Tuesday lab of the next week.  Completing each assignment will solidify your understanding of the week's topics. In addition to being a satisfying accomplishment on its own, the code you write forms a building block that contributes to a library you construct over the course of the quarter. This complete system is an accomplishment of which you will be immensely proud!

Each assignment is structured into a basic part and one or more proposed extensions. Completing the basic part is required. Doing solid work on the basic part puts you on track for earning a B grade. Adding in the optional extensions is what will boost your grade up into the A range. We encourage everyone to try out some of the extensions; they are a fun way to go further with the material and advance your understanding!

Check out our website pages [assignments](/assignments/#faq) and [course policies](/policies) for more information on assignments, grading, lateness, and collaboration.

#### YEAH! = Your Early Assignment Help
Sean will hold a YEAH session for each assignment every Thursday, during the first hour of his office hours (usually 1pm - 3pm PDT). Come join us and kickstart your work! We'll have a working version to demo and share best practices for success, highlight tricky details, warn of pitfalls to avoid, answer your questions, and more.

This week, YEAH hours will run from 12pm - 1pm PDT since Sean's office hours are shifting (just for this week!). Look out for announcements on Piazza about YEAH hours each week. YEAH

## Basic part: simple scan pattern

This assignment involves modifying the blink program showed in class.
`blink.s` is an ARM assembly language program that blinks a single
LED.  To implement a scanner, you will need to control at least 4
LEDs. Using arithmetic and loops, your assembly program changes which
LED is lit to show a scanner pattern.

### 1. Wire up your breadboard
The basic scanner should have a row of 4 LEDs. If you feel ambitious and want to make more interesting patterns, wire up 8 LEDs. Validate your breadboard wiring by testing that each LED lights up when powered.

### 2. Connect the LEDs to GPIO 20-23
The leftmost LED in your scanner should be connected to GPIO 20 of your Raspberry Pi, the second from the left to GPIO 21, and so on, such that the rightmost LED in your scanner is connected to GPIO 23 (or GPIO 27 if using 8 LEDs). Refer to the [Raspberry Pi pinout](/guides/images/pinout.pdf) to map each GPIO to the physical pin so that you correctly wire up the connections.

### 3. Get starter files
Before starting each new assignment, first do a `git pull` in your copy of the `cs107e.github.io` repository to ensure your courseware files are up to date.

```
$ cd ~/cs107e_home/cs107e.github.io
$ git pull
```

To get the assignment starter code, change to your local assignments repository, fetch any changes from the remote repository and switch to the assignment basic branch:

```
$ cd ~/cs107e_home/assignments
$ git fetch origin
$ git checkout assign1-basic
```

Once you switch to the `assign1-basic` branch, a new file `Makefile` and a folder `src` will be added to your directory. The `src` folder will always contain all your source code for each assignment, and the file named `Makefile` will always be at the root of your assignment repo. The file `src/apps/larson.s` is the file in which you will write your code for the assignment. The starter version of `larson.s` is simply a copy of `blink.s` from lab. 

{% include callout.html type="info" %}
You'll notice that `larson.s` lives in the `src/apps` subdirectory. You might think that having a hierarchy of directories for such a simple assigmnent is overkill, and indeed it is. However, as the quarter progresses and you knock down the assignments one by one, you'll build up a lot of `.c` and `.s` files in your assignments repo. Rather than dealing with a whole mess of files at a single level of your repo, we opted to organize the files into logically named subdirectories. Navigating a few extra directories now may be a little frustrating, but we think that it'll make your life much easier in the assignments to come. (The hierarchy of directories also has the advantage of showing you how C projects "in the wild" are structured.)
</div>

The starter files include a file named `Makefile`. This is a build script similar to the `doit` scripts you saw in lecture&mdash;it will use the `arm-none-eabi-as` and `arm-none-eabi-objcopy` utilities to compile your `larson.s` into a `larson.bin` file that you can install to the Pi. To use the Makefile to build your project, run `make` from the command line in the directory containing the Makefile, e.g.

```
$ make
mkdir -p build/bin build/obj
arm-none-eabi-as src/apps/larson.s -o build/obj/larson.o
arm-none-eabi-objcopy build/obj/larson.o -O binary build/bin/larson.bin
$ make install
rpi-install.py build/bin/larson.bin
```

For now, don't worry about how `make` works. We'll go through Makefiles in-depth in the next lab.

If you build the starter program and install it on the Pi, it will blink GPIO 20. This GPIO is wired to the leftmost LED of the scanner on a correctly-wired breadboard.  Confirm this on your setup and you're now ready to write your scanner program!

### 4. Configure scanner GPIOs
The starter program is a copy of the `blink.s` you studied in lab 1. This program configures the single pin GPIO 20 and enters an infinite loop to set and clear that pin. Carefully review this code and be sure you understand how it accomplishes its tasks. Ask questions if anything is unclear! Your job is to modify this program to instead blink the scanner sequence.  

A good first step is to extend the initialization code to configure all of the GPIOs your scanner uses, not just GPIO 20. Refer to section 6 of the [Broadcom ARM Peripheral](/readings/BCM2835-ARM-Peripherals.pdf) for the details on the GPIO registers.

Be sure to verify that your configuration code is correct. You can test an individual pin by changing the existing loop to blink GPIO 21 or GPIO 22 and so on instead of GPIO 20. Once you have verified that all pins are properly configured, you're now ready to tackle changing the loop body to blink the pins in the scanner sequence.

One strong recommendation from us is that you make a habit of __regular git commits__. A commit takes a snapshot of your changes and records them into your repository history. This creates a marker that allows you to later return to this version if need be. When should you commit? Any time you hit an "interesting" point in your development: after achieving a milestone, before stopping for break, after fixing a bug, before embarking on an exploratory path, and so on. Be sure to include a descriptive message with each commit. Following up a commit with `git push` will sync your local repository to your GitHub remote repository.
  
### 5. Generate scanner pattern
The loop will scan back and forth from the GPIO 20 LED to the GPIO 23 LED. Each iteration of the loop still turns on/off one GPIO, but instead of blinking the same GPIO each time, it will blink the next GPIO pin in sequence.

To receive full credit, the scanning pattern should look smooth. In particular, exactly one LED should be on at any given time, so the light appears to be "moving" between LEDs. Make sure you handle the left and right 
ends properly (that is, the leftmost and rightmost LED should be each 
on for the same amount of time that a middle LED would be on). The scanning frequency should be around 1Hz (that is, the light should bounce back and force about once per second). 

Here is a short video of our 8-LED scanner in action: 

<iframe width="420" height="236" src="https://www.youtube-nocookie.com/embed/vhDcb7lxCF4?modestbranding=1&version=3&loop=1&playlist=vhDcb7lxCF4" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

Writing clean, readable assembly code is a challenge. Commenting is essential!  Be sure to choose good label names and add named constants using `.equ` to aid readability. You should write the program without any function calls (because we haven't taught you how to create and call functions in assembly language yet). Although you cannot use functions, you can still strive for clarity in your design.

Minimize the uses of branches in assembly. Straight-line code is code without branches
and is almost always easier to understand. However, a downside of straight-line code is that there
may be a lot of code duplication. Try to factor your code to remove repeated code.
Here there is a trade-off between less code and clearer code. Above all, strive for code that can be
easily understood, whether it's by you six months from now or by us in a week when we grade your assignment.

If you have time, experiment with different ways to structure the code. (Be sure to use git commits to separately track your explorations!) Of the options you considered, which approach seems most clear to you and why?

Besides being understandable, good code should be easy to extend and maintain.
A rule of thumb for style is that if your code is well-designed, it should take little additional code to extend it from 4 to 8 LEDs, ideally just changing a constant.

Be sure to commit the final version of your code and push to GitHub.

Congratulations on completing your first step on the path to bare-metal mastery! Sit back and enjoy the show. Turn off the lights and impress your friends. 

## Extension

The basic part of this assignment should not take too long once you
understand ARM assembly.

If you want to explore further, extend the program.
The extension is more challenging, and
may involve using additional ARM assembly instructions.
It will almost certainly require more complicated code
involving conditional branches.

<!---
1 Program up 4 more patterns.
If you want ideas,
check out some of these
[patterns](https://www.youtube.com/watch?v=uUlkumlkryo#t=17).
-->

If you watch the Larson scanner carefully,
several LEDs are turned on at once.
The center one is at full brightness,
and the ones on the edge are dimmer.
How would you dim an LED? Your challenge for the assignment extension is to 
implement a scanner with LEDs aesthetically dimmed
in a way that would please Glen Larson.

You should have a central, full-brightness light that scans just like
in the basic version of the assignment, but you should visibly dim the
neighbors of that central light as it moves. Aim for a look similar to
the
[Larson Scanner Kit Demo](https://www.youtube.com/watch?v=yYawDGDsmjk).

You should have at least three distinct levels of brightness.

Before starting on an extension, be sure you have first committed and pushed a working copy of your basic code on the `assign1-basic` branch. Now create a new branch for the extension:

    $ git checkout -b assign1-extension

Commit and push your changes for the extension on this new branch. Implement the extension by modifying the `larson.s` file. DO NOT create any new files for the extension.

*Note*
When you create a new branch locally, you'll need to push that branch to your GitHub repo online. This is a process called setting the "upstream" reference for this branch in git. In order to do this, the first time you push to your new branch, you'll need to use the following special push command after adding and commiting your changes

    $ git push --set-upstream origin assign1-extension
    
After the first time you push to your new branch, you can just use the push command normally.

    $ git push

## Submit and automated check
Submit the finished version of your assignment by making a git "pull request"
from the `assign1-basic` branch to `master`, following the steps given in the
[Assignment 0 writeup](/assignments/assign0/).  If you implemented the
extension, create a separate pull request for `assign1-extension`
into `master`.

*NOTE*
If you don't create a pull request for your assignment, it won't be graded. Make sure you create a pull request for both your basic and extension branchs on each assignment. If you forget to create a pull request, there will be an automatic __0.5 points deducted from your assignment automatically.__ If you're confused about what a pull request is or why you need to create one, feel free to reach out to us either on Piazza or via the staff mailing list. 

To grade your submission, the human grader will invoke `make` to build your larson.bin file and test it on a Raspberry Pi with the LEDs plugged in. For this process to go smoothly, your project must successfully build as submitted, without any errors or warnings. There are no automated tests setup on your repo right now, but make sure that your project:

- `make` runs successfully in the clean testing environment
- You use pins 20 - 27 for your scanner (depending on how many LEDs you use)
- You have a Makefile (the same one that was included in the starter code)

If the human grader needs to go into your repo and edit your code manually for `make` to successfully run, there will be an __automatic deduction of 1 point from your assignment grade__. 
