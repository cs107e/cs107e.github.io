## Assignment 4: Simple debugger

In this assignment,
you will implement a simple debugging assistant
that will help you remotely debug code running on your Pi.

In particular, you will add a debug function `db`.
Calling `db` will cause you to enter an interactive
*read-eval-print* loop.
While inside `db`,
you will be able to inspect registers, read memory, 
and continue executing the caller.

The goals of this assignment are:

- To use the printing functions you developed in the last assignment 
to send debug information from the Raspberry Pi,
and to add additional input functions so that you can
read from the laptop.

- Write code to find values in registers from a C program.


### Basic Part: Debugger

Included in the starter code is the following example program `hello.c`.

    void notmain(void)
    {
        while(1) {
            puts("hello\n");
            db():
        }
    }


If you call `db()` as shown above,
you should get a prompt on your terminal.
Compile the starter code and test it.

    % rpi-install.py program.bin
    % screen /dev/tty.SLAB_USBtoUART 115200
    hello
    db> _

`db` should then read a line of input using the UART,
figure out what to do, do it (evaluate the input command),
and finally print the result.
This is called a read-eval-print loop.

A line consists of all the characters up to the '\n' or '\r' character.
Remember, when you are reading from the UART,
you will need to echo the characters that you type.
Also, if you type ENTER on your keyboard,
`screen` will send a return character '\r'.
You will probably want to echo both return and newline.
so that the cursor goes to the beginning of the next line.

If you just type return, print `db> ` again.

One more potential gotcha.
What happens if you mistype a command
and correct an error by typing backspace or delete?

Once you can read input from your laptop, implement the following commands.

The command `x` (examine memory) should dump 16 bytes of memory 
starting from the given address.

    db> x 8000
    8000 e1 f2 01 02 03 04 05 06 07 d0 e1 aa fe ff ff ff

The address should be given as a hexadecimal value.

In order to do this,
you will need to convert the string of digits that you
typed into screen into an integer.
Once you have the integer address, 
use it to print out the values from memory.
If you did the 
extension in the last assignment,
you should be able to reuse your `hexdump` function.

The command `i` (for information) should print out the 
current values in the registers.

    db> i
    r0  0x01
    r1  0x00
    r2  0x00
    r3  0x00
    r4  0x00
    r5  0x00
    r6  0x00
    r7  0x00
    r8  0x00
    r9  0x00
    r10 0x00
    fp  0x00
    r12 0x00
    sp  0x8000
    lr  0x8060
    pc  0x8090

Note, the values of the registers have been made up in this example.

How are you going to get the resister values from C?
*Hint:* use an assembly language function.

The final command you need to implement is 'c' (for "continue").
Continue should return from `db` and continue running your program.
This will cause "hello" to be printed,
and control passed back to `db`.

    db> c
    hello
    db>

You should use the libraries that you have created in the last assignment.
Your development directory should contain the following files,
in addition to your main program.

    % ls
    gpio.h
    gpio.c
    timer.h
    timer.c
    uart.h
    uart.c
    printf.h
    printf.c

### Extension

Add a "backtrace" function.

    db> b

Backtrace should print out the call stack.
For each frame,
you should print out the `lr` and `fp` stored in the frame.
You should then follow the frame pointers backward,
printing out the frame as you go,
until you get to the root frame (where `fp` equals 0).


