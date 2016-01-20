---
layout: page
title: 'Assignment 2: Implementing a Clock'
permalink: /assignments/assign2/
---

**Due: Monday, January 25, 2016 at 11:59PM**

This assignment involves building a clock with a 4-digit 7-segment display.

The goals of this assignment are:

- Understand bare metal programming in C on the Raspberry Pi.
- Build a simple GPIO library of useful functions.
- Learn how to use the timer.
- Understand how to refresh a 4-digit 7-segment display.

### Basic Part: A Simple Clock

1.  Wire up your breadboard with a 4-digit 7-segment display.

    Hopefully you finished setting it up in [lab](/labs/lab2). Test it so that
    you know the wiring is correct before you connect it to the Raspberry Pi.

2.  Connect the Raspberry Pi GPIO pins 20-27 to the 1K current limiting
    resistors that drive the anodes A, B, C, D, E, F, G on your breadboard.

    The 8th pin controls the colon, which is optional. Connect GPIO pins 10-13
    to the base of the transistors controlling digits 1-4. At this point if you
    output 3.3V on GPIO 20, and 3.3V on GPIO 10, you should turn on segment A
    of digit 1.

3.  Pull your private assignment repository and checkout the assign2 branch.
    Familiarize yourself with the starter code.

    You will implement two extremely useful functions in `clock.c` for
    controlling GPIO pins, `gpio_set_function` and `gpio_write`:

        void gpio_set_function(unsigned int pin, unsigned int function);
        void gpio_write(unsigned int pin, unsigned int value);

    You can find a description of the function and their parameters just above
    their declarations in `clock.c`. Implement these functions and test
    them by turning on and off various segments on different digits on your
    display.

    When implementing `gpio_set_function`, refer to the various functions
    defined in the documentation for the function select registers. In this
    assignment, we will just be using INPUT and OUTPUT, but make sure
    `gpio_set_function` works for other GPIO functions. Finally, make sure
    these functions can be called multiple times with different pins. Each call
    to `gpio_set_function` should only change the function for the pin passed
    in as an argument; other pins should remain in the same mode.

4.  Implement the logic for displaying a numerical digit on a 7-segment
    display.

    Create an array with 16 entries, one for each hexadecimal value between 0
    and 15. Each entry in the array should be a byte (8-bits). Bit 0 will
    represent segment A, bit 1 segment B, and so on. If a bit is set, then that
    segment should be lit. For example, the bit pattern for `0` is:
    `0b00111111`.

    Typically, bit 7 is used to represent `DP`, the dot. Since we won't be
    using the dot in this assignment, bit 7 should always be 0.

    Create patterns for all the digits 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C,
    D, E, F. You won't need A-F for this assignment, but they may be useful in
    the future. Test this part by writing a program that displays a number on
    the display.

5.  In order to implement a clock, we'll need some way to figure out what time
    it is. Thankfully, the Raspberry Pi includes a "system timer", an on-board
    peripheral that is continuously incremented once every microsecond behind
    the scenes, that will allow us to do just this.

    Implement the `timer_get_time` function in `clock.c` that fetches the
    current system time from the Raspberry Pi. Chapter 12 of the [Broadcom
    BCM2835 Peripherals
    Manual](http://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf)
    contains the documentation for the system timer peripheral, and the
    function's description is in `clock.c` above its declaration. Note that,
    for this assignment, we only care about the lower 32-bits of the system
    timer.

6.  Only one set of GPIO pins drives the 7 segments for all of the four digits.
    These pins are _shared_ by the four digits, and we use another, different
    set of pins to control which digit is currently active.

    We are going to drive all four digits by continuously looping over the
    digits one-by-one. In the inner loop, we will turn on a digit, wait a short
    length of time, and then turn off the digit. You might think that turning a
    digit on and off would cause it to flicker. The key is to sequence through
    the digits so fast that our eyes cannot see them changing. Good thing
    computers are fast!

    In order to refresh the display, we will use the `timer_get_time` function
    you implemented in the previous step. We have also provided a function
    `delay_us` that uses `timer_get_time` to wait for the desired number of
    microseconds.

    Implement the display refresh loop. Loop though all four digits, turning
    each on for 2500 microseconds (2.5 milliseconds). Do you see any flicker?

7.  Finally, implement the clock.

    The current time will be represented with four numbers: hours, minutes,
    seconds, and milliseconds. The inner loop should perform one refresh cycle
    of the 4-digit display using the current time, and then increment the time
    by the length of time needed to perform the refresh. Make sure the timer is
    calibrated correctly so the clock is running at the right rate.

    You may want to create two versions of your clock. One that shows minutes
    and seconds, and another that shows hours and minutes. The minutes and
    seconds version will be easier to debug initially since the display will
    change more frequently, but once it is working, you should change to hours
    and minutes to make it more like a real clock.

### Extension

One challenge with a clock is how to set the time.

Add two buttons to your clock breadboard and connect them to GPIO pins 2 and 3.
Build a user interface that allows
you to set the time. Try to design an interface that is easy to use. It can be
challenging to build an interface with just a few buttons!

Make sure to document your interface in a README.md file so that your grader
can test it out!

### Magic Files

The starter code contains an extra file `start.s`. You must have this file if
you are calling functions in bare metal programs. This file sets up the stack
pointer. We will discuss why functions need a stack next Friday in class. In
the meantime, sit tight, and don't think about it too much. The starter code
also includes a file ```memmap```, which is used by the linker. We will also
cover why that file is needed soon.
