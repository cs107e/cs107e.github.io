---
released: true
permalink: /labs/lab7/
title: 'Lab 7: Raspberry Pi, Interrupted'
toc: true
readings: |
  [Prelab](/labs/lab7/#prelab-preparation) to prep before lab.
---
{% comment %}
Task list to copy/paste when creating PR for this lab:

__Before releasing lab7:__
- [ ] Review writeup/code/checkin questions (instructor)
- [ ] Walk through (SL)
- [ ] Followup on issues from previous quarter postmortem (issue #)

__To prep for lab7:__
- [ ] 

{% endcomment %}

*Lab written by Philip Levis, updated by Pat Hanrahan and Julie Zelenski*

## Goals

The goal of this lab is to practice with interrupts in preparation for your final assignment where you will re-work your keyboard driver into an interrupt-driven version that allows the console to operate in a satisfyingly snappy and responsive manner.

During this lab you will:

- Review the support code for interrupts on the Pi.
- Write code to handle button presses using GPIO event interrupts.
- Optimize a screen redraw function (and enjoy the adrenaline rush!)
- Brainstorm possibilities for achieving world domination with an awesome bare-metal Raspberry Pi final project.

## Prelab preparation
To prepare for lab, do the following:

- Pull the latest version of the `cs107e.github.io` courseware repository.
- Clone the lab repository `https://github.com/cs107e/lab7`.
- Browse our [project gallery](https://cs107e.github.io/project_gallery/) to gather ideas and inspiration from the projects of our past students.

## Lab exercises

Pull up the [check in questions](checkin) so you have it open as you go.

### Interrupts

#### 1) Review interrupts code (20 min)

In class, we introduced the `interrupts` module that is used to configure interrupts and manage interrupt handlers. The module interface is documented in `interrupts.h` and its implementation is split into the files `interrupts.c` (C code) and `interrupts_asm.s` (assembly). The files are in the `cs107e/include` and `cs107e/src` subdirectories. You can access the cs107e directory by [browsing cs107e on github](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/) or by changing to the directory `$CS107E` on your laptop. 

Read through the code for `interrupts_init` and review the contents of the vector table and how it is installed into the correct location. Very carefully trace the critical sequence of assembly instructions in the interrupt vector used to safely transition into interrupt mode and back again. Once you get to a C function, the code proceeds pretty normally. Look at the C code which stores the client's handler function pointers and how the dispatch finds the appropriate handler to process an interrupt.

Challenge one another to understand each and every line of this code.  After you've gone through it, work through the questions below to confirm your understanding. Ask us for clarification on anything you find confusing.

+  When installing the vector table, `interrupts_init` copies eight instructions plus two additional words of data. What are those additional words and why is it essential that they are copied along with the table? The existing code copies the information using an explicit loop. This could instead be a single call to `memcpy`. What would be the correct arguments to `memcpy` to do the equivalent copy task?
 
+  How does the private helper `vector_table_is_installed` verify that initialization was successful? Find where this helper is used. What client oversight is it attempting to defend against?

+  What piece of state needs to change to globally enable/disable interrupts?

+  The supervisor stack is located at `0x8000000` and configured as one of the first instructions executed in `_start`. Where is the interrupt stack located and when it is configured?  A different approach would be to configure the interrupt stack at program start along with the supervisor stack, but doing so then would require temporarily changing to interrupt mode -- why is that switch needed?

+  How is a function "attached" as a handler for an interrupt source? If multiple handlers are attached to the same source, how does the dispatch operation determine which one processes the interrupt? What is the consequence if no handler is found to process an event? Why does the dispatcher assert that the handler return value matches the event cleared status?  What client oversight is this attempting to defend against?

#### 2) Set up a button circuit (15 min)

Set up a simple, one-button circuit on your breadboard. Connect one side of the button to GPIO pin 20 and the other side to ground. Connect your Pi to a HDMI monitor.

<img title="Button circuit" src="images/button-circuit.jpg" width="300">

Configure the button circuit so that the default state of the pin is high (1) and pressing the button brings it low (0). The way to do this is to use a "pull-up resistor".  (We won't use a *physical* resistor; instead, we'll tell the Pi to use an internal resistor.)   In the open circuit when button is not pressed, the resistor "pulls up" the value to 1.  When the button is pressed, it closes the circuit and connects the pin to ground. The value then reads as 0. This is like the behavior of the PS/2 keyboard clock line.  

Change to directory `code/interrupts`.  Review the code in `button.c`.  The program operates in a loop that waits for a button press and then redraws the screen. You are to fill in the implementation of the empty `configure_button` and `wait_for_click` functions.

Implement `configure_button` to set the button gpio as an input and turn on the internal pull-up resistor. You may want to look at the similar code you used in `keyboard_init` to configure the gpio pins for your keyboard driver.

Implement `wait_for_click` to detect a button press. The function should:

+ Wait for a falling edge on the button gpio, i.e. wait until the pin goes from 1 to 0 (checking its state with `gpio_read`).

+ Use `printf("+")` to report the press.


Compile and run the program. Press the button and you get a printed `+` and the screen redraws. Each redraw cycles the background color from red->white->blue.

1. If you click the button multiple times in quick succession, some of the presses are missed. You get neither a printed `+` nor a screen redraw. Why does that happen?

You'll note that redrawing the screen is glacially slow. If we sped that up (which we will do later in this lab!), it would cause us to miss fewer events, but that's not a complete fix. An approach based on "polling" has compromises in design awkwardness and wasted cycles. Rather than having to repeatedly check the button state, let's ask the CPU to nudge us when a button event occurs. Interrupts to the rescue!

#### 3) Write an interrupt handler (30 min)

You are now going to rework the program to detect button presses via interrupts. 

Edit the loop in `main` to remove the call to `wait_for_click`. Compile and re-run. The program should now repeatedly redraw the screen, cycling the colors red->white->blue. This appears to completely dominate the CPU and leave no time for anything other than screen redraw. If we manage to jam an interrupt into there, then we will know we can stuff one anywhere!

There are several steps to configuring and enabling interrupts:

- Modify `configure_button` to enable detection of falling edge events. Consult the header [gpioextra.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/gpioextra.h) for documentation on the function `gpio_enable_event_detection`.
- Define a new handler callback function that checks whether the button has a pending event and if so, processes and clears the event. Review the functions in `gpioextra.h `for checking and clearing the event status. For now, use `printf("!")` to process.
- In `main`, call `interrupts_init` to initialize the interrupts module and call `interrupts_attach_handler` to attach your handler function to the interrupt source for gpio events `INTERRUPTS_GPIO3`
- After all is ready, `main` calls `interrupts_global_enable` to turn on interrupt generation.

Talk this over with your tablemates and ensure that you understand what each step does and why it's necessary. 

Compile and run the program. As before, the loop main appears to be continuously redrawing, but pressing the button causes a quick detour to print a `!`.  You may even see a string of `!` from a single press to due mechanical bounce in the switch.

Let's change `main` instead to only redraw in response to a button press. 

Coordination across regular and interrupt code can be done using global state.  Declare a global variable `static int gCount` to count the number of presses. Edit the handler function to increment `gCount` and print the count along with the `!`. Edit the loop in `main` to compare the current value of `gCount` to the last known value, and only redraw whenever it has updated.

Compile and run your program. Confirm that each button press is individually detected and counted (including multiple events from a noisy switch).

1. The variable `gCount` must be declared `volatile`. Why? Can the compiler tell,
by looking at only this file, how control flows between main and the interrupt handler? Will the compiler generate different code if `volatile` than without it? Will the program behave differently? Test it both ways and find out!


Now, edit your handler to comment out the step that clears the event and have the handler return false. Compile and run the program and see how this changes the behavior. What changes and why?

When you're done, discuss and answer the following questions with your 
neighbors.

1. What changes if `gCount` is not declared `volatile`?

1. Describe what is done by each step of configuring and enabling interrupts. What would be the effect of forgetting that step?

1. What happens if the handler does not clear the event before returning?

#### 4) Use a ring buffer queue (15 min)

Look carefully at the output printed by the program and you'll note that although every event is detected and counted, the number of redraw iterations is not one-to-one with those updates because the value of `gCount` can update multiple times between checks. 

To track all updates and process each one by one, we can use a queue to communicate between the interrupt handler and `main`. The handler will enqueue each update to the queue and the main will dequeue to read them from the queue. In this way, we will never miss an update coming from the interrupt handler.

How to rework the code:

- Review the [ringbuffer.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/ringbuffer.h) header file and [ringbuffer.c](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/src/ringbuffer.c) source for documentation on the ring buffer queue. This ADT maintains a simple queue of integer values implemented as a ring buffer.
- Declare a global variable of type `rb_t *rb` and edit `main` to initialize `rb` with a call to `rb_new`.  
- Edit the handler to store the updated counter's value into the queue with a call to `rb_enqueue`.
- Edit `main` to retrieve an update from the queue with a call to `rb_dequeue`.(This is used in place of the previous code that read the global variable and compared to previous value to detect update).

Make the above changes and rebuild and run the program. It should now redraw the screen once for each button press in one-to-one correspondence.

When you're done, take a moment to verify your understanding:

1. Why is the significance of the return value from `rb_dequeue`? Why is it essential to pay attention to that return value?
1. Why might you want the handler to enqueue and return instead of doing
   the actual task (e.g. redraw) directly in the handler?
1. With this change, is it now necessary for `gCount` to be declared `volatile`? Does the ring buffer need to be declared `volatile`?  Why or why not?

### Project brainstorm and team speed-dating (20 minutes)

Visit our [project gallery](https://cs107e.github.io/project_gallery/) to see a sampling of projects from our past students. We are __so so proud__ of the creations of our past students -- impressive, inventive, and fun! You'll get started in earnest on the project next week, but we set aside a little time in this week's lab for a group discussion to preview the general guidelines and kindle your creativity about possible directions you could take in your project. If you have questions about the project arrangements or are curious about any of our past projects, please ask us for more info, we love to talk about the neat work we've seen our students do. If you have ideas already fomenting, share with the group to find synergy and connect with possible teammates. Project teams are typically pairs, more rarely solo or trios.

### Need for speed (20 min)

As a fun bonus exercise, let's take a look at what you can do when you're done
writing correct code: optimization. With this, your code can start moving
wickedly fast -- seriously.

Change directory to `code/speed` and review the source in the `speed.c`
file.

The program has the `redraw` function from the interrupts exercise, along with some timer scaffolding to count the
ticks during the function's execution. The purpose of `redraw` is to draw every pixel in the screen in the same color.

The given version works correctly, but is naive to a fault. Build and run the
code as given to get a baseline tick count. *It is possible to gain more than a 1000x speedup over the
redraw0 function!*

Take a stepwise approach, so you can measure the effect of a given
modification in isolation:

1. **Duplicate the previous function and tweak it.** For example, make a copy of `redraw0` and rename it `redraw1` before proceeding. Make a small change to the code that you think will impact performance.

2. Make a rough prediction about the expected effect on runtime.

3. Now run and time the new version to see whether your intuition matches
   the observed reality. Where the results surprise you, examine the
   generated assembly to see if you can figure out why there is a different
   effect than you expected.

Repeat this process, each time advancing from the best version so far and
making another small change.

Below are some suggested avenues to explore:

+ Hoisting an unnecessarily repeated operation outside the loop is always good idea. Those calls to `gl_get_width` and `gl_get_height` on each loop iteration have gotta go! 
+ There are a __lot__ of calls to `gl_draw_pixel`. Each is incurring the overhead of a function call and the (redundant) bounds checking within the call.  If you bypass `gl`, get the draw buffer from `fb` and write each pixel directly to the framebuffer, it will have a significant effect.
+ Does changing the order the pixels are accessed make a difference, i.e. instead of looping row by column, what if you loop column by row?
+ What about looping over the pixels as a 1-d array instead of nested loop in 2-d?
+ Here's something that can be tried without changing the code: edit the Makefile to enable various levels of compiler optimization and
rebuild and re-run. What difference do you observe between `-O0`, `-Og`, `-O2` and
`-Ofast`?
+ Think about where the function spends time. Recall that every instruction 
contribute a cost: are there ways to change the function so that it does the 
same work with fewer instructions? Take a look at the assembly in `speed.list` to see where
the effort is going. It takes just a few instructions to write a pixel but each loop iteration adds the overhead cost to increment, compare, branch. How could you change the loop to issue fewer of these
overhead instructions?

How big of an improvement were you able to make overall? Where did you get the biggest bank for your buck?

## Check in with TA

At the end of the lab period, call over a TA to [check in](checkin) with your progress on the lab.

