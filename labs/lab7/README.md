---
released: true
permalink: /labs/lab7/
title: 'Lab 7: Mango Pi, Interrupted'
attribution: Lab written by Philip Levis, Pat Hanrahan and Julie Zelenski
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
- [ ] Ensure monitor/cable per pair avail in lab

{% endcomment %}

## Goals

The goal of this lab is to get you started with interrupts in preparation for
your final assignment.

During this lab you will:

- Review the support code for interrupts on the Pi,
- Write code to handle button presses using GPIO event interrupts, and
- Brainstorm possibilities for achieving world domination with your final project.

## Prelab preparation
To prepare for lab, do the following:

- Review the interrupt support code we introduced in lecture (all source files in directory `$CS107E/src`);
    - `interrupts_asm.s` assembly instructions for interrupt vectors, enable/disable
    - `interrupts.c` library module that configure/manages interrupt sources and trap_handler dispatch to registered handler per-source
    - `gpio_interrupt.c` library module used to configure/manage GPIO interrupts and dispatch to registered handler per-pin
    - `ringbuffer.c`. library module that implements queue as ringbuffer, safe for shared access by one enqueuer + one dequeuer
    - `hstimer.c`. library module for countdown timer

- Browse our [project gallery](/project_gallery/) to gather ideas and inspiration from the projects of our past students.

## Lab exercises

### Pull lab starter code

Change to your local `mycode` repo and pull in the lab starter code:

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull code-mirror lab7-starter
```
### Interrupts

#### 1) Review interrupt assembly

In the first lecture on interrupts, we went over the low-level mechanisms. The `interrupts` module is used to configure interrupts and manage handlers at a global level. The module interface is documented in [interrupts.h](/header#interrupts) and its implementation is split into the files [interrupts.c](/src#interrupts) (C code) and [interrupts_asm.s](/src#interrupts_asm) (assembly). These source files are available in the directory `$CS107E/src`.

Start by reviewing the `interrupts_asm.s` file. The functions `interrupts_global_disable` and `interrupts_global_enable`
flip the bits in the `mstatus` and `mie` CSRs to enable/disable machine-mode interrupts. The additional routines access the CSRs used by the interrupt system. In order to read/write the CSRs, we have to use the special `csr` instructions that can only be accessed via assembly.

+ What piece(s) of state needs to change to globally enable/disable
interrupts?

+ Which of the assembly routines is used to install the code to be called when an interrupt fires?

+ Which other CSRs are accessed by the assembly code. What are those CSRs used for?


Let's now consider the C function `trap_handler`. The function is declared `__attribute__((interrupt("machine")))` Review the [gcc documentation](https://gcc.gnu.org/onlinedocs/gcc/RISC-V-Function-Attributes.html) to learn about function attributes. The generated assembly will include a prolog/epilog to safely enter and exit interrupt processing. The general gist is shown below:

```
trap_handler:
    addi    sp,sp,-144  // prolog to save register values on stack
    sd      ra,136(sp)
    sd      t0,128(sp)
        ...
    sd      t6,8(sp)
    addi    s0,sp,144

    // body of function

    ld      ra,136(sp)  // epilog to restore registers to saved values
    ld      t0,128(sp)
    ...
    ld      t6,8(sp)
    addi    sp,sp,144
    mret
```

+ Of the 32 general purpose registers, how many registers are being saved to the stack? Why those registers are not others?

+ How does the memory being used for the stack frame `trap_handler` relate to the stack memory being used by the regular execution?

+ At what address does `mret` resume execution? Where does it get that address from?

You're ready for this check-in question[^1].

#### 2) Review interrupt dispatch

The `trap_handler` routine in `interrupts.c` is invoked for any exceptional event. If the event cause is external interrupt, it dispatches to the registered handler. All other event causes are unrecoverable fatal exception, so we simply print an error message and call `mango_abort`.

In our second lecture on interrupts, we looked at how to dispatch an event to its associated handler. The design for dispatch in both `interrupts` and `gpio_interrupt` uses an array of function pointers, one per-index. The client implements a handler function and registers it with the dispatcher. The dispatcher stores the client's function pointer in the array at the associated index. When an event occurs, the dispatch invokes the handler at that index. In the top-level `interrupts` module, the interrupt source number is the index into the array of handlers. In the `gpio_interrupts` module, the pin index within the gpio group is used as the index.

There is a neat performance trick that applies here. To identify which event is pending, the dispatcher scans the pending/status registers to find the first bit that is set. If you were implementing that scan in C, you might loop and shift/test each bit indvidually. A more streamlined version could do fancy bit twiddling, such as Kernighan's algorithm, or employ on a lookup table. Such versions might take tens or even hundreds of cycles.

A better way to implement is to drop down to assembly leverage bitwise tricks to count leading zeros. We are using the hand-rolled assembly provided by gcc (`__builtin_clz`). Enthusiastic hackers compete to see if they can [outperform it](https://www.reddit.com/r/RISCV/comments/132s19s/hand_optimised_riscv_assembly_language_clz/).  The recently ratified RISC-V [Zbb extension](https://drive.google.com/file/d/11-dKxnp7yfl9L3HESXGCtYl90dFKGTzE/view) adds the `clz` (count leading zeros) instruction that counts in as few as 3 cycles. (The processor we use predates this extension, so not available on Mangp Pi).  Reducing the time it takes to find a pending interrupt from 100 to 3 cycles, (an improvement of 33x!) is a big benefit to every single interrupt.  This kind of throughput boost is why instructions like `clz` exist. Neat!

+ How is a function "registered" as a handler with a dispatcher? How does the dispatcher know which handler to call for a given event? Can there be multiple handlers registered for the same event?

+ An `aux_data` pointer can be stored with the handler. That pointer is later passed as an argument to the handler when invoked. What is the purpose of an `aux_data` pointer?

Confirm your understanding of interrupt dispatch with this check-in question[^2].

#### 3) Set up a button circuit

Now let's get going on using interrupts in a program. Set up a one-button circuit on your breadboard. Connect one side of the button to gpio PB4 and the other side to ground. Connect your Pi to a HDMI monitor.

![button circuit](images/button-circuit.jpg){: .zoom -w-50}

The button circuit is configured for a default state of high (1) by activating the Pi's internal pull-up resistor on the button GPIO.  When the button is not pressed, the resistor "pulls up" the value to 1.  When the button is pressed, it closes the circuit and connects the pin to ground. The value then reads as 0.

The `main` function in `lab7/button/button.c` sits in a loop, waiting for a button press and then redrawing the screen. Fill in the implementation of the empty `wait_for_click` function to operate by _polling_. It should:

1. Wait for a falling edge on the button gpio, i.e. watch for the pin level to transition from 1 to 0 ( using `gpio_read` to read the level).

2. Increment the `gCount` global variable and announce the event with a `uart_putstring("click!")`

Compile and run the program. When you click the button, the message is printed and the screen redraws to show the incremented count of clicks. This version of the program is always redrawing or waiting for a click, but it's either one or the other. While waiting for a button press, the screen redraw is paused. While redrawing the screen, no button presses are detected. Ideally, we want the program to do both tasks concurrently.

- If you click the button multiple times in quick succession, some of
the presses are missed. You get neither a printed message nor a screen
redraw and these clicks are not included in the count. Why does that happen?

You'll note that redrawing the screen is quite slow. If we speed
that up, it would cause us to miss fewer events, but we still have to spin waiting for a press and still can miss events. Interrupts will solve this problem.

#### 4) Write a button handler

Remove the call to `wait_for_click` from the loop in `main`. Compile and re-run. The program now repeatedly redraws the screen.  If you click the button, there is no response. The program never calls `wait_for_clock` and won't observe any change in the GPIO pin, it's 100% occupied with drawing.

You are now going to rework the program to intercept those button clicks as interrupts.

Start by reviewing the documentation for the library modules you will use:
+ [gpio_interrupts.h](header#gpio_interrupts)
    * Gpio interrupts module
+ [interrupts.h](/header#interrupts)
    * Top-level interrupts module

Remember there are __three__ layers to configuring and enabling interrupts:
1. Configure/enable specific event (in this case, gpio event)
    + Configure interrupt (negative edge for button gpio)
    + Implement handler function to process event
        * Handler must match prototype `void handle_click(uintptr_t pc, void *aux_data)`.
        + Use the `aux_data` parameter to pass a message to your handler. The handler treats the received pointer as  `char *` and calls `uart_putstring` to output the message.
        + Be sure that your handler clears the event!
2. Configure/enable interrupt source
    + Init gpio interrupt module
    + Register handler with gpio interrupt. For `aux_data`, pass the message string "[PUT-YOUR-NAME-HERE] has interrupt mojo" so the handler will print this message.
    + Enable gpio interrupt.
3. Configure/enable at top-level
    + Init interrupts module (at start of program)
    + Enable global interrupts (when entire system ready)

The order that you do these operations can be very important: think
carefully about each action, revisiting the lecture slides/code if
you need to.  Talk this over with your tablemates and ensure that you
understand what each step does and why it's necessary.

Compile and run the program. If you have done everything correctly, the program continuously redraws as before, but now whenever you click the button, it prints a message in your terminal to congratulate your prowess with interrupts and the click count increments. You get the best of both worlds: your long-running computation can be written as a simple loop, yet the system is immediately responsive to input.

Once you have it working, go back and intentionally make various errors, such as doing steps out of order, forgetting a step, or doing a step twice.  Seeing the observed consequences of these mistakes now may help you to identify them in the future. You're ready for another check-in question[^3].

#### 5) Coordinate between main and interrupt

You want to change the program to now redraw once in response to a button click rather than continuously update. This requires that the interrupt and main code share state.

Edit the code within the loop in `main` to only call `redraw` if the count of clicks `gCount` has changed since the last redraw. Save the count used at last redraw in the local variable `drawn` and compare to `gCount` to determine when a redraw is necessary.

The count is being stored in the global variable `gCount`. The handler increments it and the `main` reads the value and compares to saved count.  `gCount` is not currently declared `volatile`. Should it be? Why or why not?  Can the compiler tell, by looking at only this file, how control flows between main and the interrupt handler? Will the compiler generate different code if `volatile` than without it? Will the program behave differently? Test it both ways and you'll be ready to answer this check-in question[^4].

#### 6) Use a ring buffer queue

Watch carefully as the program executes and you'll note that every click is detected and counted, but the count of redraw iterations is not one-to-one with those updates. Multiple clicks can occur before the `main` loop gets around to next checking the value of `gCount`.

To track all updates and process each one by one, we can use a queue to communicate between the interrupt handler and `main`. The handler will enqueue each update to the queue and `main` will dequeue each update. Because the queue stores every individual update posted by the interrupt handler, we can be sure that we never miss one.

How to rework the code:

- Review the [ringbuffer.h](/header#ringbuffer) header file and source file [ringbuffer.c](/src#ringbuffer) to see the provided ring buffer queue. This ADT maintains a queue of integer values implemented as a ring buffer.
- In the `main` function, declare a variable of type `rb_t *rb` and initialize with a call to `rb_new`.  Use the `rb` pointer as the `aux_data` pointer when registering the handler.
- Edit your handler to now cast the `aux_data` parameter to type `rb_t *`.  In the handler, enqueue the updated value of count to the ring buffer by calling `rb_enqueue`.
- Edit `main` to use `rb_dequeue` to retrieve each update from the queue. This replaces the previous code that compared `gCount` to the saved value to detect a change in click count. Be careful with the second argument to `rb_dequeue` -- you are passing the address of an int and `rb_dequeue` will write the dequeued value to that memory location (i.e. used as "out" parameter).

Make the above changes and rebuild and run the program. It should now redraw the screen once for each button press in one-to-one correspondence, including patiently processing a backlog of redraws from a sequences of fast presses.

When you're done, take a moment to verify your understanding:

- Why is the significance of the return value from `rb_dequeue`? Why is it essential to pay attention to that return value?
- With this change, is it now necessary for `gCount` to be declared `volatile`? Does the ring buffer need to be declared `volatile`?  Why or why not?

You're ready for the final check-in question[^5].

### Project brainstorm and team speed-dating

Visit our [project gallery](/project_gallery/) to see a sampling of projects
from our past students. We are __so so proud__ of the creations of our past
students -- impressive, inventive, and fun! You'll get started in earnest on
the project next week, but we set aside a little time in this week's lab for a
group discussion to preview the general guidelines and spark your creativity
about possible directions you could take in your project. If you have questions
about the project arrangements or are curious about any of our past projects,
please ask us for more info, we love to talk about the neat work we've seen our
students do. If you have ideas already fomenting, share with the group to find
synergy and connect with possible teammates. Project teams are most typically
pairs, although occasionally we have allowed solo or trios by special request.


### Bonus speed exercise
In past quarters, we have used some of the time in lab7 for explorations in performance optimization. We don't think there will be time for it today, but we linked the [performance materials](speed) if you want to check it out for fun.

## Check in with TA

Before leaving the lab, check in with us and discuss any challenges to answering the questions in the lab. If you do not feel confident about your responses, please ask us about it too!

[^1]: The trap_handler passes the interrupted pc into the handler function as the first argument. How does it access the value of the interrupted pc?

[^2]: If there is no handler registered for a pending event, what will the dispatcher do with the event?

[^3]: What happens if an interrupt handler does not clear the event before returning?

[^4]: What (if any) is the consequence of not declaring the `gCount` variable as `volatile`?

[^5]: Why might you want the handler to enqueue an update and return instead of doing the actual task (e.g. redraw) directly in the handler?
