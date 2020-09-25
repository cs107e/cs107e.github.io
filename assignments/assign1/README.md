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
We will hold a YEAH session for each assignment every Thursday. Come join us and kickstart your work! We'll have a working version to demo and share best practices for success, highlight tricky details, warn of pitfalls to avoid, answer your questions, and more. Look out for YEAH hour announcements on Piazza.

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

Next, you should make sure you have the most recent version of the starter code from the `assignments-mirror` repo. To update your local repo, you should pull any changes from the mirror repo into your master branch:

```
$ cd ~/cs107e_home/assignments
$ git checkout master
$ git pull starter-code master
```

Now that you have the starter code, you should create a new branch named `assign1-basic`:

```
$ git checkout -b assign1-basic
```

You can check that you're on this new branch by running `git branch`. 

Now that your repo has the latest version of the starter code and is on the
right branch, take a peek at what's in your repo using `ls`. 

The `src` directory contains the source code for each assignment and is split up
into four subdirectories: `apps`, `boot`, `lib`, and `tests`. 
- `apps` contains the app--the program that you'll actually run on your Pi--for 
  each of the assignments that you'll complete this quarter. 
- `boot` contains a few files required to successfully load and launch a program.
  You can ignore these files for now. `
- `lib` contains the modules that you'll implement as part of your effort toward
  building a fully functioning computer by the end of quarter.
- `tests` contains the unit tests that you'll write to test your modules. 

You won't write any library modules for this first assignment, so you can focus
in on `src/apps`. Inside of it, you'll find `larson.s`, which is simply a copy of
`blink.s` from lab. You'll modify this file as you implement your Larson
scanner.

Take a peek at the `makefiles` directory. You'll notice that it contains seven files,
all ending in the `.makefile` suffix. Each file corresponds to a different
assignment. You can think of a makefile as a recipe that contains the
instructions for how to build your code. To invoke a makefile, we use the `make`
program, which expects to find a filed named `Makefile` in the directory from
which `make` was executed. You may notice that you're missing such a file. To
resolve this, we're going to create a symbolic link (also called a soft link)
that will point from `Makefile` to `makefiles/assign1.makefile`. This will
simultaneously please the `make` program and remove the need for us to maintain
two separate versions of our current makefile. To create this symbolic link, do
the following:

```
$ cd ~/cs107e_home/assignments
$ ls
$ ln -sf $(pwd)/makefiles/assign1.makefile $(pwd)/Makefile
$ ls -l 
```

Before running `ln` (the link command), there will be no `Makefile` at the top
level of your assignments repo. After running `ln`, you'll notice that there is
a `Makefile`, and that it points to `makefiles/assign1.makefile`. Neat! You can
now build your code using `make` and run the resulting program on your Pi using
`make install`:

```
$ make
mkdir -p build/bin build/obj
arm-none-eabi-as src/apps/larson.s -o build/obj/larson.o
arm-none-eabi-objcopy build/obj/larson.o -O binary build/bin/larson.bin
$ make install
rpi-install.py build/bin/larson.bin
```

If you're wondering how `make` and makefiles work, sit tight! We'll cover
makefiles in-depth in the next lab.

Once you build and run the starter program on your Pi, confirm that it blinks
GPIO 20. Now, you're ready to write your scanner!

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

Writing clean, readable assembly code is a challenge. Commenting is essential!  
Be sure to choose good label names and add named constants using `.equ` to aid readability.
Because you don't know how to write functions yet in assembly, you'll need to
write your program without the aid of this useful abstraction. Even without
functions, you can still attain a clear and intuitive design.

To maximize the clarity of your assembly code, minimize the use of branches in. 
Straight-line code is code without branches and is almost always easier
to understand. However, a downside of straight-line code is that there may be a 
lot of code duplication. Try to factor your code to remove repeated code.
Here there is a trade-off between less code and clearer code. Above all, strive 
for code that can be easily understood, whether it's by you six months from now 
or by us in a week when we grade your assignment. Either way, enjoy tackling the
interesting compromises inherent in software design.

If you have time, experiment with different ways to structure the code. (Be sure
to use git commits to separately track your explorations!) Of the options you 
considered, which approach seems most clear to you and why?

Besides being understandable, good code should be easy to extend and maintain.
A rule of thumb for style is that if your code is well-designed, it should take 
little additional code to extend it from 4 to 8 LEDs, ideally just changing a constant.

Congratulations on completing your first step on the path to bare-metal mastery!
Sit back and enjoy the show by turning off the lights and impressing your
friends.


## Submission
Because you've been committing regularly throughout the development process--even
if you haven't been, we're going to pretend that you have because it helps 
all of us sleep at night--all you need to do now is ensure that your final changes made
it into your last commit, push those commits to your remote repo on GitHub, and
open a pull request on GitHub. You created a new branch locally, which means you
need to run the special version of the `git push` command that we say in assignment 
0. This is a process known in Git as setting the "upstream" reference for the new
local branch. 

```
$ git push --set-upstream origin assign1-basic
```

Once you push for the first time to your new branch, you can use plain old `git
push` on all subsequent pushes to this branch.

Once you've pushed your final version, open a pull request in GitHub just like
you did in assignment 0. Ensure that you're opening a pull request to merge
`assign1-basic` into `master` and give the pull request an intuitive title like
"Submit assign1-basic".

{% include callout.html type="danger" %}
Don't forget to open a pull request! We need you to open a pull request so that
we can grade your assignment.
</div>

If you want a refresher on the steps required to open a pull request, see 
the [Assignment 0 writeup](/assignments/assign0/)

## Extension

If you enjoyed the basic part and want to explore further, go ahead and extend
the program. The extension is more challenging, and may involve using additional
ARM assembly instructions. It will almost certainly require more complicated code
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

Before starting on an extension, be sure that you've submitted the basic portion
of the assignment. Now create a new branch for the extension:

```
$ git checkout -b assign1-extension
```

{% include callout.html type="info" %}
Remember to set the upstream reference for your new branch. This will require
using the fancier version of the `git push` command that we saw above: 
```
$ git push --set-upstream origin assign1-extension
```
</div>

Once you've finished the extension, be sure to open a new pull request for
merging `assign1-extension` into `master`. Again, give the pull request an
intuitive title like "Submit assign1-extension".

## Grading
To grade your submission, your beloved CAs will invoke `make` to build your `larson.bin`
file and test it on a Pi with the LEDs plugged in. For this process to go smoothly,
please ensure the following:

- You can successfully invoke `make` without any build warnings or errors.
- You use pins 20-23 or 20-27 (depending on how many LEDs you chose to use).

If the CA grading your assignments needs to go into your repo and edit your code
manually for `make` to successfully run, there will be an __automatic deduction 
of 0.5 points from your assignment grade__. More importantly, we will be sad.
