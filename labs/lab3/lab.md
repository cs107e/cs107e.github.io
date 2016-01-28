---
layout: page
title: 'Lab 3: Debugging'
permalink: /labs/lab3/
---

*Lab written by Pat Hanrahan*

### Goals

During this lab you will:

1. Learn how to simulate the ARM processor using `arm-none-eabi-gdb`
   and debug your programs in it.

2. Set up your console cable to work with the UART on the Raspberry
   Pi, and learn how to use `printf` to debug your programs.

3. Learn how to write a multiplatform program that can run on both
   your laptop and on the Pi.

The upcoming assignment will require you to write the most complex
program so far in the course, so we want to introduce you to some
useful debugging techniques.

To complete the lab, you must answer the questions in the
[checklist](checklist).

### Pre-lab preparation

To prepare for this lab, you should read the
[gdb](/guides/gdb) tutorial about how to debug the ARM
processor using `gdb` in simulate mode.

### Lab exercises

#### Debugging Technique #1: gdb

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
(CPSR)*. [Here is a page which documents this register and how its bits are structured.](http://www-mdp.eng.cam.ac.uk/web/library/enginfo/mdp_micro/lecture3/lecture3-1.html)

To see how the 32 bits of CPSR stores information, let's walk through the 'blink.s' program again. This time, write down the value of the CPSR register at 3 instances:

1. Before the delay loop
2. In the middle of the loop
3. After the loop

These values, however, are in hex form so you will need to convert
them to binary to see the bit representation. Once you do so, pay
particular attention to the four condition code bits (which are the
four most significant bits), N, Z, C, and V. Record your answers to
the CPSR questions at the end of the gdb guide on the
[checklist](checklist).

#### Debugging Technique #2: printf using serial communication

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

##### Using printf

Change to the `cs107e.github.io/labs/lab3/code/printf` directory.

Now that your Pi can communicate with your computer, you can use the
printf we provide you in libpi.a to debug program state.

For example, you can call `printf("value: %d\r\n", 10);` to print the
number 10 or `printf("value: %c\r\n", 'a');` to print the letter a. To
learn more about how to use printf, check out
[the documentation for it here](http://www.tutorialspoint.com/c_standard_library/c_function_printf.htm).

Open `print.c` in your text editor.

Edit 'print.c' to do the following three things. Be sure you
do these *after* the `uart_init()` call and initial delay, though, so
that you have time to start `screen` after running the program.

1. Use printf to print out the value of the GPFSEL2 register in hex form. 

2. Use `gpio_set_function` to make pins 20 and 21 output pins.

3. Print out the value of GPFSEL2 again.

Then restart your Pi, `make` and `make install` the program again, and
use `screen` as before to see your program's output. Recall that in
order to see the value of each bit, you have the convert the value
that was printed into binary. Record the answer on the checklist.

#### Debugging Technique #3: cross compiling and run on your laptop

Now we will show you how to write a C program that can run on both the
Pi and on your laptop. Then you can quickly rerun your program on your
laptop after making changes, and compare your printf's behavior to
your computer's printf.

Change to the `cs107e.github.io/labs/lab3/code/debug` directory.

We have provided you a test program, `program.c`, along with a
Makefile. Open `program.c` in your text editor.

##### Run on your laptop

We will start by showing how you can run your C programs on your
laptop, too, with some work.

Notice the use of `#ifdef`, `#else`, `#endif` preprocessor
directives. These conditions are evaluated by the C preprocessor before
`program.c` reaches the compiler. We are using them to specify sections
of code that will execute on your laptop, as opposed to on the Pi.

This way, we can have one C file that works on both platforms. When
you run `program` on your laptop, it will be able to run both
`my_puts` (for the Pi), and the system implementation of `puts`. This
allows us to compare their output.

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

Wait the initial delay time of 10 seconds, and then the program should start printing to your console.

Check that our implementation of `my_puts` matches the system's `puts`
in behavior (they should be outputting the same thing).

Press Ctrl-C to terminate the program.

##### Print from the Pi

Finally, let's load this program onto the Pi itself, then see what
it's printing.

Make sure your serial cable is connected and your Pi is ready to
receive the program. Again, there is a 10 second delay before the program will start printing.

```
$ make
...
$ make install
sent True
$ screen /dev/tty.SLAB_USBtoUART 115200
```

You should see the same thing you saw when running it on your computer
locally.

#### Extension: gdb simulation and the stack

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
