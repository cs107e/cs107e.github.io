## Assignment 1: Implementing a Clock

This assignment involves building a clock with
a 4-digit 7-segment display.

The goals of this assignment are

- Understand bare metal programming in C on the Raspberry Pi

- Build a simple GPIO library of useful functions

- Understand how to refresh a 4-digit 7-segment display

- Learn how to use the timer

### Basic Part: A Simple Clock

1 Wire up your breadboard with a 4-digit 7-segment display.
Hopefully you finished setting it up in [lab](../../lab/lab2/lab.md).
Test it so that you know the wiring is correct before
you connect it to the Raspberry Pi.

2 Connect the Raspberry Pi GPIO pins 20-27
to the 1K current limiting resistors that drive
the anodes A, B, C, D, E, F, G on your breadboard.
The 8th pin controls the colon, which is optional.
Connect GPIO pins 10-13 to the base of the transistors
controlling digits 1-4.
At this point if you output 3.3V on GPIO 20,
and 3.3V on GPIO 10, you should turn on 
segment A of digit 1.

3 Familiarize yourself with the starter code in `code/clock`. 
Copy all files in `code/clock` to the assign2 directory of 
your private assignment repo.

In this assignment,
you will implement 
two extremely useful functions for controlling GPIO pins,
`pinmode` and `pinwrite`. 

    void pinmode( int pin, int mode );
    void pinwrite( int pin, int value );

Documentation for these functions is in the starter code.
Implement these functions, and test them
by turning on and off various segments on different
digits on your display.

4 Implement the logic for displaying a numerical 
digit on a 7-segment display.
Create an array with 16 entries,
one for each hexdecimal value between 0 and 15.
Each entry in the array should be a byte (8-bits).
Bit 0 will represent segment A, bit 1 segment B, and so on.
If a bit is set,
then that segment should be lit.
Create patterns for all the digits
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F.
You wouldn't need A-F for this assignment,
but they may be useful in the future.
Test this part by writing a program that
displays a number on the display.

5 There is only one set of GPIO pins 
driving the 7 segments. 
These pins are shared by all four digits.
We are going to drive all four
digits by continously 
looping over the digits one-by-one.
In the inner loop,
we will turn on a digit, 
wait a short length of time,
and then turn off the digit.
You might think that turning a digit on and off
would cause it to flicker.
The key is to sequence through the digits
so fast that our eyes cannot see them changing.
Good thing computers are fast.

In order to refresh the display, we will use the *system timer*.
The file clock.c contains a function `timer` that reads 
the system timer register.
The system timer is a peripheral on the Raspberry Pi.
The timer is being continuously incremented 
once every microsecond behind the scenes,
For more information about the system timer, read Chapter 12 of the
[Broadcom BCM2835 Peripherals Manual](http://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf).

Each time you call the function `timer` 
it will return the current value of 
the lowest 32-bits of the 64-bit system timer.
We have also provided a function `delay_us` that uses the `timer` 
to wait for the desired number of microseconds.

Implement the display refresh loop.
Loop though all four digits, turning each on 
for 2500 microseconds (2.5 milliseconds).
Do you see any flicker?

6 Finally, implement a clock.
The current time will be represented with four numbers:
hours, minutes, seconds, and milliseconds.
The inner loop should 
perform one refresh cycle
of the 4-digit display using the current time,
and then increment the time by the length of time 
needed to perform the refresh.
Make sure the timer is calibrated correctly so the
clock is running at the right rate.

You may want to create two versions of your clock.
One that shows minutes and seconds,
and another that shows hours and minutes.

### Extension

One challenge with a clock is how to set the time.

Add two buttons to your clock breadboard.
Build a user interface
that allows you to set the time.
Try to design an interface that is easy to use.
It can be challenging to build an interface with just a few buttons!


### Magic File

The starter code contains an extra file `start.s`. 
You must have this file if you are calling functions 
in bare metal programs.
This file sets up the stack pointer.
We will discuss why functions need a stack next Friday in class. 
In the meantime, sit tight, and don't think about it too much.

