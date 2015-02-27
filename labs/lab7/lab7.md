## Lab 7: Raspberry Pi, Interrupted
# Lab written by Philip Levis

### Learning goals
During this lab you will:

1. Get interrupts working on your Raspberry Pi
1. Display button presses on the screen

The goal of the lab is for you to get experience with enabling
interrupts and having them interact with your main loop.
Please complete the [check list](checklist.md) as you work through the lab.

We've provided several support libraries (GPIO implementations, timers,
interrupt initialization, etc.) to make this easier. Look at what's
available to you before
getting started so you don't have to repeat work.

There are two parts to this lab: getting interrupts working and writing
a simple interrupt program.

### Lab exercises

1. __Pull the lab 7 project.__

    To start this lab, find the `courseware/labs/lab7/code` directory.
    You may need to pull from the `cs107e/courseware` repository
    to get the latest code.

    Inside the lab7 directory is a binary named `interrupt-display.img`.
    This is the binary of the program I displayed in the second interrupts
    lecture, which shows the state of many registers on the Pi. You
    can use this to see if interrupts are pending and enabled. When
    you create a falling edge on pin 23 it increments count by 0x1000
    and increments count on each iteration of its while loop by 1.

1. __Compile the project and inspect its assembly.__

    The default program, `notmain.c` sets up everything so that interrupts
    will invoke its function `int_handler`. Open `notmain.elf.list` and
    answer the questions in part 1 of the checklist.

1. __Write a simple interrupt.__
    Connect your test circuit from lab 6 to GPIO pin 23. In `notmain.c`,
    configure your program so that a falling edge on GPIO pin 23 will
    trigger an interrupt. Don't forget to set the GPIO pin
    to have a pull-up resistor. 

    In the interrupt handler, increment a counter.
    In your main program `while(1)` loop, display the value of the
    counter using `snprintf` and `gfx_draw_string`. Be sure to check and
    clear the pin 23 event in the interrupt handler, or it will fire
    forever and you will not refresh the screen! Show this to the TA
    to check off part 2 of the checklist.

1. __Display interrupts.__
    Make 4 copies of your test circuit with 4 different buttons.
    Hook them up to 4 different GPIO pins. Configure your Pi so it
    has an interrupt on either a falling or rising edge (whenever
    the state of a button changes).

    In the interrupt handler,
    clear any pending events and use `gpio_pin_read` to check
    the state of each button. For each time the button is pressed or
    released, increase a counter by 1. Display this counter on your
    screen in your main loop. How many button presses can you trigger
    in a single iteration of your main loop? Does this number seem
    higher or lower than the actual number of times you pressed buttons?
    Show this to the TA to check off part 3 of the checklist.


## Check off with TA
At the end of the lab period, submit the checkoff form and ask your lab
TA to approve your submission so you are properly credited for your work.

