---
layout: page
title: 'Lab 3: Debugging'
permalink: /labs/lab3/
---

*Lab written by Pat Hanrahan*

### Goals

During this lab you will:

1. Learn how to simulate the ARM processor using `gdb`.

2. Set up your console cable to work with the UART on the Raspberry
Pi.

3. Use multiple techniques to debug a program: running on your system
   instead of the Pi, using the gdb simulation, and printing from the
   real Pi through the console cable.

To complete the lab, you must answer the questions in the
[checklist](checklist).

### Pre-lab preparation

To prepare for this lab, you should read the
[gdb](/guides/gdb) tutorial about how to debug the ARM
processor using `gdb` in simulate mode.

### Lab exercises

#### 1. Simulate the blink program using gdb

The goal of the first exercise is to teach you how to use gdb in ARM
simulation mode. This is useful as a way of learning what happens
when ARM instructions are executed, and to debug programs.

Change to the `cs107e.github.io/labs/lab3/code/blink`
directory. Assemble and link the program using `make`:

    $ make
    arm-none-eabi-as -g blink.s -o blink.o
    arm-none-eabi-ld blink.o -o blink.exe

**Follow all the steps in the [gdb guide](/guides/gdb).** This guide
walks you through how to setup gdb to simulate the ARM processor.

The final part of this exercise, as stated in the guide, is to
understand the *current program status register
(CPSR)*. [Here is a page which documents this register.](http://www-mdp.eng.cam.ac.uk/web/library/enginfo/mdp_micro/lecture3/lecture3-1.html)
Pay particular attention to the four condition code bits at the top,
N, Z, C, and V. Record your answers to the CPSR questions at the end
of the gdb guide on the [checklist](checklist).

#### 2. Using the console cable with the Raspberry Pi

*Make sure your cp2102 drivers are installed and work properly.*

If you haven't done this, 
follow the instructions in the [bootloader guide](/guides/bootloader).

*Insert the usb serial point into a usb port on your laptop.*

Check whether it appears as a tty device.

On a Mac:

    $ ls /dev/tty.SLAB_USBtoUART
    /dev/tty.SLAB_USBtoUART

On Linux:

    $ ls /dev/ttyUSB0
    /dev/ttyUSB0

You can also type "dmesg" or use "tail -f" on whatever log your kernel writes to.  E.g., 

    $ tail -f /var/log/kern.log  # on ubuntu
    [output deleted]
    Jan 27 16:23:45 ThinkPad-W530 kernel: [116656.523573] usb 1-1.2: cp210x converter now attached to ttyUSB0

If the drivers are not installed, this tty port will not appear.
If you remove the usb serial breakout port from your usb port,
the device will not appear.

*Wire the TX pin to the RX pin on the usb serial breakout board.*

Connecting TX to RX is called a *loop back*.
In loop back mode,
the signals sent out on the TX pin are wired to the RX pin.
This causes characters sent out to be echoed back.

![loop back](images/loopback.jpg)

Start a terminal program and connect to the tty port from above.  For
example,

    $ screen /dev/tty.SLAB_USBtoUART 115200

The screen should be cleared and the cursor positioned
in the upper left hand corner.
Type in some characters.  What happens?
What happens if you push return on your keyboard?

To exit screen, type Control-A followed by Control K.
You should see the following message.

    Really kill this window [y/n]

Typing `y` should return you to the shell.

    [screen is terminating]

*Test printing on your screen from the Raspberry Pi.*

Run `git pull` in your `cs107e.github.io` folder to download the code
examples from the latest lecture.  Change to the directory
`cs107e.github.io/lectures/serial/code/uart`.

    $ cd cs107e.github.io/lectures/serial/code/uart
    $ ls
    Makefile    hello.c     start.s     uart.c      uart.h
    $ make
    arm-none-eabi-gcc -I../include  -Wall -O2 -nostdlib -nostartfiles -ffreestanding -c hello.c 
    arm-none-eabi-gcc -I../include  -Wall -O2 -nostdlib -nostartfiles -ffreestanding -c uart.c 
    arm-none-eabi-as  start.s -o start.o 
    arm-none-eabi-ld -L../lib start.o uart.o hello.o -o hello.exe -lpi
    arm-none-eabi-objdump -D hello.exe > hello.list
    arm-none-eabi-objcopy hello.exe -O binary hello.bin

Now, wire up the usb serial breakout board 
so that TX on the breakout board is connected to RX on the Raspberry Pi.
Also RX on the breakout board to TX on the Raspberry Pi.

Download the `hello.bin` to the Raspberry Pi 
using the bootloader by running `make install`.

Power up the Raspberry Pi, and start `screen`.

    % screen /dev/tty.SLAB_USBtoUART 115200
    Hello, laptop
    Hello, laptop
    Hello, laptop
    Hello, laptop
    Hello, laptop
    Hello, laptop
    Hello, laptop

This will print forever.
To stop it, exit `screen` (holding the control key down and typing
"ad" is a quick way; if you haven't reset the pi you can also 
reattach later by doing "screen -r")

#### 3. Debug a program

The upcoming assignment will require you to write the most complex
program so far in the course. We want to introduce you to some useful
debugging techniques.

Change to the `cs107e.github.io/labs/lab3/code/debug` directory.

We have provided you a test program, `program.c`, along with a
Makefile. Open `program.c` in your text editor.

##### Run on your laptop

We will start by showing how you can run your C programs on your
laptop, too, with some work.

Notice the use of `#ifdef`, `#else`, `#endif` preprocessor
directives. These are evaluated by the C preprocessor before
`program.c` reaches the compiler. We are using them to choose sections
of code that will work on your laptop, as opposed to on the Pi.

This way, we can have one C file that works on both platforms. When
you run `program` on your laptop, it will try both our implementation
of `puts`, which we've called `my_puts`, and the system implementation
of `puts`, so that we can compare their output.

Which parts of `program.c` will get included if the value
`LOCAL_TEST` is defined? What if that symbol is not defined?

Next, look at the Makefile. In particular, notice the `local` and
`run-local` targets. What C compiler command, instead of
`arm-none-eabi-gcc`, is used to build a program for your laptop? What
argument is given to your computer's C compiler to define the
`LOCAL_TEST` value?

Finally, run the program:

```
$ make local
$ ./program
```

Wait the initial delay time of 10 seconds (this is so that you have
time to start `screen`), and then the program should start printing.

Check that our implementation of `my_puts` matches the system's `puts`
in behavior (they should be outputting the same thing).

Press Ctrl-C to terminate the program.

##### Print from the Pi

Finally, let's load this program onto the Pi itself, then see what
it's printing.

Make sure your serial cable is connected and your Pi is ready to
receive the program.

```
$ make
...
$ make install
sent True
$ screen /dev/tty.SLAB_USBtoUART 115200
```

You should see the same thing you saw when running it on your computer
locally.

Finally, you'll use the printf we provide you in libpi.a to debug
program state. Edit program.c to do these three things (make sure you
do these after the `uart_init()` call and initial delay, though, so
that you have time to start `screen` after running the program):

1. Use printf to print out the value of the GPFSEL2 register. You can
call `printf("value: %d\r\n", 10);` to print the number 10, for example.

2. Use `gpio_set_function` to make pins 20 and 21 output pins.

3. Print out the value of GPFSEL2 again.

Then restart your Pi, `make` and `make install` the program again, and
use `screen` as before to see your program's output. Record the answer
on the checklist.

##### Extension: gdb simulation and the stack

_This section is optional._

Let's debug this program in gdb's simulator like we debugged the blink
program earlier. We'll also use the `-tui` option which shows a source
listing on top.

```
$ arm-none-eabi-gdb -tui program.exe
```

Check back to the [gdb guide](/guides/gdb) if you don't remember how
to use gdb. Remember to `target sim` and `load`.

Now set a breakpoint at the line with `puts("hello")`, inside the
`#ifdef`. Then run the program. Where does gdb actually break? Why?

Set another breakpoint at the first line inside the `my_puts`
function with `b 19`. Continue the program. 

```
(gdb) b 19
Breakpoint 1 at 0x8014: file program.c, line 19.
(gdb) cont
```

gdb should break where you set the breakpoint. Now let's examine the
backtrace.

```
Breakpoint 1, my_puts (s=s@entry=0x84dc "hello") at program.c:19
(gdb) backtrace
#0  my_puts (s=s@entry=0x84dc "hello") at program.c:19
#1  0x00008050 in main () at program.c:32
(gdb) 
```

Frames have numbers. The current frame is numbered 0, and corresponds
to the invocation of function `my_puts`. Frames for caller functions
have higher numbers.

```
(gdb) info frame
Stack level 0, frame at 0x7ff8:
 pc = 0x8014 in my_puts (program.c:19); saved pc = 0x8050
 called by frame at 0x8000
 source language c.
 Arglist at 0x7ff0, args: s=s@entry=0x84dc "hello"
 Locals at 0x7ff0, Previous frame's sp is 0x7ff8
 Saved registers:
 r4 at 0x7ff0, lr at 0x7ff4
(gdb) info args
s = 0x84dc "hello"
(gdb) info locals
No locals.
```

We can also inspect caller functions' locals.

```
(gdb) up
#1  0x00008050 in main () at program.c:32
```

This moves up to function #1, which is the function `main`. `main`
called `my_puts`.

```
(gdb) info args
No arguments.
(gdb) info locals
No locals.
```

Try playing around with adding locals to `main`. Why might locals that
you define in `main` not appear in the debugger?

Finally, let's look at the limitations of debugging with the
simulator.

Set a breakpoint at the line containing `DELAY(1);` at the bottom of
the loop body in `main`. Then continue the program's execution
again.

Does gdb reach that breakpoint? Why not? How might you work around
this problem to debug a program like this?

### Stack intuition

Functions often need space (e.g., for variables or to store the return
address of their caller before they call another function).    There's
nothing special about this space, and we could allocate it as we
would any other memory.  However,
functions calls are frequent, so we want them as fast as possible.
Fortunately they have two
properties we can exploit for speed:
(1) when they return all memory they allocated is considered dead
(2) they return in LIFO order.  As a result
for speed, people have converged on using a contiguous array of memory
(called ``stack'' because of its LIFO usage).  Roughly speaking it
works as follows:

1. At program start we allocate a fixed-sized stack and set a
pointer (the stack pointer) to its beginning.

2. At each function call, all the memory the function needs is
allocated contiguously and all-at-once by simply incrementing
the stack pointer.

3. At each function call return, all the memory the function
allocated is then freed all-at-once by simply decrementing
the stack pointer.

OSes generally provide ways to dynamically grow the stack, but we will
ignore this here.

This organization is such a clear winner that compilers have explicit
support for it (and do the pointer increment and decrement) and
architecture manuals the rules for how to do so.
(This basic data structure appears in many other contexts when all the
data for a given purpose or type can be freed all at once.  )
If you know "malloc()" and "free()" one way to compare this method
and those functions is that the fastest malloc() you can do is a pointer
increment and the fastest free() a pointer decrement. 
