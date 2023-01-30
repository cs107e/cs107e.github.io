---
released: true
permalink: /labs/lab3/
title: 'Lab 3: Debugging and Testing'
toc: true
readings: |
  [Prelab](/labs/lab3/#prelab-preparation) to prep before lab.
---

<script>
$().ready(function() {
    var elems = document.getElementsByClassName('language-terminal');
    for (const elem of elems) elem.className += ' terminal-gdb';
});
</script>
{% comment %}
Task list to copy/paste when creating PR for this lab:

__Before releasing lab3:__
- [ ] Review writeup/code/checkin questions (instructor)
- [ ] Walk through (SL)
- [ ] Re-take photo of loopback on latest CP2102
- [ ] Followup on issues from previous quarter postmortem (issue #390)

__To prep for lab3:__
- [ ] Be extra careful when reviewing exercise 1, make sure nothing has changed to invalidate the typescript embedded in writeup
- [ ] Make copies of gdb reference sheet to hand out in lab
{% endcomment %}

*Lab written by Pat Hanrahan, updated by Julie Zelenski*

## Goals

During this lab you will:

- Learn how to use `arm-none-eabi-gdb` in simulation mode. 
Simulation mode is also a good way to learn more about how the ARM processor executes instructions.
- Use gdb to trace through function call/return and examine registers and memory.
- Learn how to use `screen` with the USB-serial adapter. Also understand how the UART peripheral works on the Raspberry Pi.
- Think a little bit about the ethical implications of software testing.
- Implement a few simple C-string operations and use a combination of
unit testing and gdb simulation to debug your work.

These exercises are useful preparation for your next assignment which is to implement `printf` for your Pi.

## Prelab preparation

To prepare for lab, do the following:

1. Read our [guide to using gdb in simulation mode](/guides/gdb). Print out a copy of the [gdb reference card](/readings/gdb-refcard.pdf) to have on hand.
1. Read Sections 1, 2, 3.6 (up to but not including 3.6.2), 4 of [Medical Devices: The Therac-25](http://sunnyday.mit.edu/papers/therac.pdf). This famous paper describes a series of software bugs in a medical device that resulted in several patient fatalities. These accidents were an important milestone that proved the ethical need for rigorous esting and sometimes formal verification of software. It also greatly motivates research subfields on these topics.

## Lab exercises

Exercises marked below as "essential" must be successfully completed in order to be ready for Assign 3. The other exercises are "nice to have". If you are running short on time during lab, okay to skim and return to later on your own time or during OH.

1. Debugging with GDB (__essential__)
2. Serial Communication (__nice to have__)
3. Talk about Therac-25 (__nice to have__)
4. C-Strings (__essential__)
5. gdb and Testing (__essential__)


### 0. Pull lab starter code

Before starting each new lab, first do a `git pull` in your $CS107E repository to ensure the courseware files are up to date.

```console
$ cd $CS107E
$ git pull
```

Now cd to your local `mycode` repo and pull in the lab starter code:

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull --allow-unrelated-histories starter-code lab3-starter
```

### 1. Debugging with gdb

This first exercise  is to practice using the `gdb` debugger in ARM simulation mode.
A debugger allows you to observe and manipulate a running program and its program state. To debug bare metal program executing on remote processor requires a complicated setup and special hardware (JTAG), so in this class we use `gdb` on our local computer to test our program running in simulation and revert to simple use of
`printf` to debug when actually running on the Pi.

`gdb` is an amazingly powerful and useful tool. When debugging C code,
proficiency with `gdb` can improve your debugging speed by a factor of 10,
or in the hands of an expert, a factor of 100. It gives you complete
visibility into a program: if you know where to look, you can find out
exactly what is happening and what your bug is. Mastering the debugger is just
as important as mastering your editor and compiler.

#### 1a) Use `gdb` in simulation mode

We will demonstrate `gdb` on a simple example program. 
Change to the directory `lab3/simple` directory and review the program in `simple.c`. 

Build the program using `make`. Note that it is the ELF 
file that we use in conjunction with the gdb simulator, 
not the raw binary file that we have been running on the actual Pi.

Run gdb on `simple.elf`. 

```console?prompt=(gdb),$
$ arm-none-eabi-gdb simple.elf
GNU gdb (GDB) 9.2
       ... blah blah blah ...
(gdb) 
```

Within `gdb`, connect to the simulator and load the program:

```console?prompt=(gdb)
(gdb) target sim
Connected to the simulator.
(gdb) load
Loading section .text, size 0x134 lma 0x8000
Start address 0x8000
Transfer rate: 2464 bits in <1 sec.
```

Set a *breakpoint* on the `main` function and start executing the program:

```console?prompt=(gdb)
(gdb) break main
Breakpoint 1 at 0x80b0: file simple.c, line 35.
(gdb) run
Starting program: .../lab3/simple/simple.elf 
Breakpoint 1, main () at simple.c:35
```

The debugger stops the program when it reaches the breakpoint. The `next` command executes
the next line of C source.
The command `print d` shows the value of `d`
after the call completes. 

```console?prompt=(gdb)
Breakpoint 1, main () at simple.c:35
35 {
(gdb) next
38      int d = diff(x, y);
(gdb) next
39      int f = factorial(7);
(gdb) print d
$1 = 74
(gdb) print f
$2 = <optimized out>
```

Note that when `gdb` shows your program stopped at a line, 
it's stopped  _before_ that line has executed. At the end of the sequence shown above, the program is stopped before calling 
`factorial`. When we try to print f, the debugger
reports that it is not (yet) accessible because we have not yet executed the line where it is declared/initialized.

Sometimes you want to step __into__ the code of the function being called.
To do this, use `step` instead of `next`. Put another
way, `next` executes the entire next line in the function
you're in, while `step` executes the next line of code, 
which may be in a different function.

Use `run` to restart the program
and then use `step` once you hit the breakpoint.

```console?prompt=(gdb)
(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Breakpoint 1, main () at simple.c:35
35	{
(gdb) step
38        int d = diff(x, y);
(gdb) ⏎
diff (a=a@entry=33, b=b@entry=107) at simple.c:31
31        return abs(b - a);
(gdb) ⏎
abs (v=v@entry=74) at simple.c:8
8     return result;
```

The return/enter key(`⏎`) causes `gdb` to repeat the last command (in above sequence, it will `step` two more times).

We first stepped into `diff` then took two steps in `abs` and execution is now stopped at the return statement.

When stepping through code, gdb displays the next line of code to be executed. To see more context, use the `list` command
```console?prompt=(gdb)
(gdb) list
4
5   int abs(int v)
6   {
7       int result = v < 0 ? -v : v;
8       return result;
9   }
10 
11  int factorial(int n)
12  {
```

Use `continue` to resume executing the program. While the program is executing, type `Control-c` to interrupt the running program and return control to gdb. Use the `backtrace` command to see where the program was executing when it was interrupted:

```console?prompt=(gdb)
(gdb) continue
Continuing.
^C
Program received signal SIGINT, Interrupt.
0x0000800c in hang ()
(gdb) backtrace
#0  0x0000800c in hang ()
#1  0x0000800c in _start ()
Backtrace stopped: previous frame identical to this frame (corrupt stack?)
```

The above information tells you that the program is stopped in `hang` which is called from `_start`. Review the code in `start.s` and `cstart.c` to remind yourself of what happens in a C program before and after `main()`. If currently in `hang`, the program has finished and is in the final "holding pattern". This is the normal behavior for a C program that has successfully run to completion. Learn to recognize how this situation is presented in the debugger. You hope to be seeing a lot of successful program completion!

When debugging a function, a common workflow is to

  1. `break` on the function in question. `run` until you hit the breakpoint.
  1. Use `next` to step through each line of its code, inspecting variables to see where
     the problem occurs.
  1. If the next line of code is a call to a subroutine and you suspect the problem could be inside that call, use `step` to drop down into it.  If you `next` through a call and realize that you wish you had used `step` instead, use `run` to start over from the beginning and get another chance.
  1. Recursively apply rules 2-3 until you find the bug.

<a name="1b"></a>
#### 1b) Use `gdb` to access stack frames
There are gdb commands that allow you to trace function calls and view stack frames.  Let's try them out!

Use `delete` to delete any existing breakpoints and set a breakpoint at the `abs` function:
```console?prompt=(gdb)
(gdb) delete
Delete all breakpoints? (y or n) y
(gdb) break abs
Breakpoint 2 at 0x8010: file simple.c, line 8.
(gdb) run
Breakpoint 2, abs (v=v@entry=74) at simple.c:8
```

You are now stopped at the breakpoint on `abs`. The command `disassemble` with no arguments lists the ARM instructions in the context where the program is currently executing. The instruction marked `=>`is the next one to be executed.
    
```console?prompt=(gdb)
(gdb) disassemble
Dump of assembler code for function abs:
=> 0x00008010 <+0>: cmp r0, #0, 0
   0x00008014 <+4>: rsblt   r0, r0, #0, 0
   0x00008018 <+8>: bx  lr
End of assembler dump.
```
Note that the first instruction of `abs` is at
address `0x8010`, as we expected.
We asked for a breakpoint on the function `abs` and gdb converted our request to `0x8010` which corresponds to the address of the first instruction of `abs`. A breakpoint set at `0xAddr` will stop the program just before executing the
instruction at `0xAddr`. 

Use the gdb `backtrace` to show the sequence of function
calls leading to here:

```console?prompt=(gdb)
(gdb) backtrace
#0  abs (v=v@entry=74) at simple.c:8
#1  0x000080a8 in diff (a=a@entry=33, b=b@entry=107) at simple.c:31
#2  0x000080c0 in main () at simple.c:38
```

The backtrace shows that the function `abs` has been called by `diff`
from line 31, which in turn was called by `main` from line 38.  The
numbers on the left refer to the *frame*.  The innermost frame is
numbered 0, and corresponds to the currently executing function, in this case, `abs`. Frames for caller functions have higher numbers. The `info frame` command prints a summary of the current stack frame:

```console?prompt=(gdb)
(gdb) info frame
Stack level 0, frame at 0x7ffffe0:
 pc = 0x8010 in abs (simple.c:8); saved pc = 0x80a8
 called by frame at 0x7ffffe8
 source language c.
 Arglist at 0x7ffffe0, args: v=v@entry=74
 Locals at 0x7ffffe0, Previous frame's sp is 0x7ffffe0
```

The `info locals` and `info args` commands give more information about the stack frame's arguments and local variables:

```console?prompt=(gdb)
(gdb) info args
v = 74
(gdb) info locals
result = 74
```

`gdb` lets you choose which frame on the call stack you wish to inspect.
The command `up` selects the frame that is one step up the call stack (e.g. frame of caller) and `down` moves back to callee.

```console?prompt=(gdb)
(gdb) up
#1  0x000080a8 in diff (a=a@entry=33, b=b@entry=107) at simple.c:31
(gdb) info args
a = 33
b = 107
(gdb) info locals
No locals.
(gdb) down
#0  abs (v=v@entry=74) at simple.c:6
(gdb) info args
v = 74
```

Another useful `info` command is `info reg` which displays the values currently in the Pi's registers. 
```console?prompt=(gdb)
(gdb) info reg
r0             0x4a                74
r1             0x6b                107
r2             0x8138              33080
r3             0x8138              33080
r4             0x4a                74
r5             0x0                 0
r6             0x0                 0
r7             0x0                 0
r8             0x0                 0
r9             0x0                 0
r10            0x0                 0
r11            0x0                 0
r12            0x0                 0
sp             0x7ffffe0           0x7ffffe0
lr             0x80a8              32936
pc             0x8010              0x8010 <abs>
cpsr           0x600000d3          1610612947
```
What value is currently in `r0`? Why does `r0` contain that value? (Consider: at what point in the program execution are we stopped right now? What was the last use of `r0`?) What value is in `r1`?

You can access a single register by using the syntax $regname, e.g. `$r0`.

```console?prompt=(gdb)
(gdb) print $r0
$2 = 74
```
{% comment %}
For further practice with gdb and stack frames, now try tracing and exploring with the `factorial` and `make_array` functions.

The `factorial` function operates recursively. Set a breakpoint on `factorial` so gdb will stop on each recursive call.  Each time you hit the breakpoint, use `backtrace` to see how the stack changes as you get deeper into the recursion. Try moving `up` and
`down` and use `info frame` and `info args` to view individual stack
frames.  Note how each invocation has its own copy of the parameter `n`. 

The function `make_array` demonstrates how the stack is used
for storage of local variables. A local variable of size 32 bits or fewer (i.e. simple int) is likely stored in a register without the overhead of  writing to stack memory. Larger data
structures, such as arrays and structs, that do not fit in a register must be stored on the
stack. Set a breakpoint on the `make_array` function. Use `info locals` to see the array contents at the start of the function. Are
the array elements initialized to any particular value?  Step through
the loop a few times and use `info locals` to see how the array is
updated. 

The goal of all this mucking about in gdb is to solidify your understanding the mechanics of function calls and the runtime stack. If there are portions you don't yet understand, ask your tablemates or the staff to resolve those questions now.

{% endcomment %}

Now introduced to `gdb`, you'll want to incorporate it into your development process and to practice until you become comfortable and fluent.  Gdb can help you learn more about how your C code translates to generated assembly, and being able to observe and manipulate your program while it is executing will be an invaluable aid when tracking down bugs.

In software development, only a small fraction of your time goes into writing the code, the rest is taken up in testing and debugging. These are important skills to hone and adept use of `gdb` can streamline your efforts by a significant factor. Developing your debugging superpowers will pay off many times over!

At this point, you should be able to answer the first check-in question[^1]

### 2. Serial communication
#### 2a) Loopback test

Your laptop communicates over a serial interface when sending a program to the bootloader. To understand what is going on, let's do a simple *loop back* test with your USB-serial adapter.

Insert the USB-serial adapter into a USB port on your laptop and identify which `tty` (teletype) device is assigned to the port. If you use the command `rpi-run.py` with no arguments; it responds with a device path of the form `/dev/your-tty-device-here`.

```console
$ rpi-run.py
Found serial device: /dev/ttyS3
```
Disconnect the two jumpers between the RX and TX of the USB-serial adapter and the TX and RX GPIO pins on the Pi.

Use a single jumper to connect TX to RX on the USB-serial adapter in loop back mode, as shown in this photo:

![loop back](images/loopback.jpg){: .zoom}

In loop back mode,
the signals sent out on the TX pin are wired straight to the RX pin. Reading from the RX pin will read the characters sent over TX.

`screen` is a program used to communicate over a tty device. Open `screen` on your USB-serial tty device and establish a connection 
at the baud rate of 115200. 

```console
$ screen /dev/your-tty-device-here 115200
```

> __Permission error on WSL?__ On WSL, if screen fails due to a permissions error, run the command `sudo chmod -R 777 /var/run` to enable the correct permissions. After doing this, try running screen again. You should only need to enable the permissions one time, screen should operate normally thereafter. 
{: .callout-warning}

When screen opens, it clears your terminal and positions the cursor
in the upper left corner.
Type some characters.  What happens?
What happens if you type return on your keyboard?

To close the connection, type `Control-a` followed by `k`.
You should see the following message.

```console
Really kill this window? [y/n]

Type `y` to exit screen and return to the shell.
[screen is terminating]
```
#### 2b) Echo test

Re-connect the TX/RX jumpers between the USB-serial and the Raspberry Pi. Remember the RX of the USB-serial connects to the TX of the Pi, and vice versa (the connections are __not__ TX-TX and RX-RX).

Change to the directory `lab3/echo` and build the program. 

The `rpi-run.py` that sends a program to the bootloader can optionally open a communication channel with the running program.  The command `rpi-run.py -s` uses `screen` to create a 2-way channel that allows your laptop to send and receive data with the Pi.  Try the `-s` flag now:

```console
$ rpi-run.py -s echo.bin
```

The commands send the `echo.bin` program to run on the Pi, and then will automatically open `screen` in your terminal. As you type, your characters are being sent to the program running on the Pi and the output from the Pi is echoed back to your terminal. 

While you continue typing, have your partner gently unplug the jumper from the RX pin on your USB-serial and then re-connect it. What changes? Why does that happen? Answer this check-in question[^2]

Use `Control-a` `k` to exit screen.

#### 2c) UART/printf test

Change to the directory `lab3/uart-printf`.

```console
$ cd lab3/uart-printf
$ ls
Makefile  hello.c
```
Review the code in `hello.c`. This program uses the `uart_putstring` function
to send characters using the TX pin on the Raspberry Pi. 

The command `rpi-run.py -p` creates a 1-way channel that echoes data received from the Pi.  The Makefile already includes the `-p` flag when invoking `rpi-run.py`, so `make run` is one-stop shopping to rebuild the program, send to the Pi, and print the received output.

```console
$ make run
Found serial device: /dev/cu.SLAB_USBtoUART
Sending `hello.bin` (1128 bytes): .........
Successfully sent!
hello, laptop
hello, laptop
hello, laptop
hello, laptop
hello, laptop
We <3 printf!

rpi-run.py: received EOT from Pi. Detaching.
```
This program sends an EOT (end of transmission) character that tells your laptop to close the communication channel. You can also type `Control-c` on your laptop to manually close.

The function `uart_putstring` can be used to output a string constant, but what is much more useful is the ability to output formatted strings, e.g. `printf`. For example, the call `printf("Today is %s %d\n", monthname, daynum)` 
inserts the month string and day number into the output. To learn more about how to use printf, check out the standard library [printf documentation](http://www.tutorialspoint.com/c_standard_library/c_function_printf.htm).

In Assignment 3, you will implement your own version of `printf`.  With a working `printf`, your programs will be able to report on their state and super-charge your debugging. What a big improvement over trying to communicate everything via just two measly LEDs!

Open `hello.c` in your text editor and edit the `main` function to try out `printf`:

1. Add a call to `printf` inside the loop body that prints the value of `i` on each loop iteration.

1. Outside the loop, use `printf` to print the value from `*FSEL2` in hex format. 

2. Use `gpio_set_output` to make pins 20 and 21 output pins.

3. Print the value of `*FSEL2` again.

Reset your Pi, and run `make run` again to see your program's output. How does the hex value stored in `FSEL2` change after changing the pin functions?

### 3. Talk about Therac-25

Talk with your neighbor(s) about the Therac-25 article. A few questions to consider:
  - Can you imagine scenarios in which a bug in a Raspberry Pi serial library 
  might cause harm? For example, scenarios in which a safety-critical program
  assumes the text output from a Pi is correct?
  - You might design and test your code under certain usage assumptions,
  then release it as open source and advertise all of its excellent
  properties. Suppose someone reads your claims then uses your code in settings 
  you did not anticipate and it fails. The failures result in millions of
  dollars of damages. Do you bear any responsibility? What would influence
  the degree of responsibility you hold? What might you do to help prevent 
  such harm from happening?

### 4. C-strings

For this exercise, you will work with C-string functions and get further practice with unit-testing and debugging.
Change to the `lab3/strings` directory and open the `cstrings.c`
file in your editor.

The first string operation to look at is `strlen`. This function returns the count of characters in a C-string. How is it possible to determine where a C-string ends? Is the null terminator included in the count? 

Review the given (correct) code for `strlen` in `cstrings.c`.  It uses array subscripting on a variable declared as a pointer. Why is this legal?  What does it mean to access the nth array element of a pointer variable?

The code for `strlen` is simple enough that you might feel confident calling it correct "by inspection", but let's use our unit testing strategy to truly confirm the behavior of the executing function.

Read through the test cases in the `test_strlen`.
Compile the program and run it on the Pi (with `make run`, not `make test` in this case). You should get the green light of success for passing all the tests. It is a correctly-implemented `strlen` function, as advertised.

Now, let's move on the `strcpy` operation. This function copies the characters from one string to another:

```c
char *strcpy(char *dst, const char *src);
```

Spoiled programmers who work on hosted systems receive `strcpy` as part
of the standard library, but we must implement it ourselves for the Pi.

Functions in the standard library each have a man page that documents the function's use and behavior. Bring up the man page for `strcpy`.

```console
$ man strcpy
```

Read the man page and then try implementing `strcpy` yourself. Does `strcpy` copy only the non-null characters or is the terminator included?  If you're not sure, read the man page to confirm the expected behavior.

When you are ready to test your `strcpy`, review the test cases in the `test_strcpy` function. Uncomment the call to `test_strcpy` in `main()`. Use `make run` to rebuild the program and run it on the Pi.

If you get the red flash of doom, dig in to find out what's gone wrong
and work to resolve the issue. Don't move on until all
tests pass and you earn your green light merit badge.

### 5. Gdb and testing

You can also test a program by running within gdb in simulation mode. Let's try that now on the cstrings program from the previous exercise.

#### 5a) Debug strlen

Edit `strlen` to intentionally plant a bug, such as changing the function to always return `7`. This will cause test failures in `test_strlen`. Use `make run` to build the program and run on the Pi and you get the flashing red LED that indicates a failed assert.

Let's learn how that failed assert is presented under the debugger. Get the buggy program under gdb and run it. There is no flashing red light; the simulator is not talking to your Pi nor its peripherals. Your Pi doesn't even need to be connected! 

What do you see in gdb? There is some output about a failed assertion and then the program appears to be stuck. Type `Control-c` to interrupt the program and use `backtrace` to see where the program is stopped.

```console?prompt=(gdb)
File cstrings.c, line 32: Assertion 'strlen("green") == 5' failed.
^C
Program received signal SIGINT, Interrupt.
0x000084e8 in timer_delay_us (usecs=usecs@entry=200000) at timer.c:15
15  timer.c: No such file or directory.
(gdb) backtrace
#0  0x000084e8 in timer_delay_us (usecs=usecs@entry=200000) at timer.c:15
#1  0x00008530 in timer_delay_ms (msecs=msecs@entry=200) at timer.c:19
#2  0x00008454 in pi_abort () at pi.c:43
#3  0x000080ac in test_strlen () at cstrings.c:32
#4  0x00008268 in main () at cstrings.c:69

```

A-ha! When an assert fails, it calls `pi_abort` to flash the red light. The above backtrace tells you that the program is waiting in the delay loop within `pi_abort`.  Given the simulator does not emulate the timer or GPIO peripherals, `pi_abort` behaves as a no-action infinite loop. By looking further into the backtrace, we learn that the failed assertion occurred on line 32 of the `cstrings.c` file (or some nearby line number, depending on how long your `strcpy` implementation was). Use `list` to see that code now:

```console?prompt=(gdb)
(gdb) list cstrings.c:30
27 
28 void test_strlen(void)
29 {
30     char *fruit = "watermelon";
31 
32     assert(strlen("green") == 5);
33     assert(strlen("") ==  0);
34     assert(strlen(fruit) == 2 + strlen(fruit + 2));
```

This allows us to pinpoint exactly which assert failed (rather than have to comment-in-and-out tests one by one to find it). Hooray for gdb!

Restore `strlen` to its correct implementation, rebuild and run again under the debugger. All tests should pass. As expected, there is no green light from the simulator, but once again the program appears stuck. Type `Control-c` to interrupt the program and use `backtrace` to see what's going on. What evidence confirms that the program successfully ran to completion? 


> __Tip__ Typing `Control-c` will interrupt the executing program and return control to the debugger. `backtrace` will show where the program was executing when it was interrupted. 
{: .callout-info}

Learn to recognize these two common situations: 
+ a successful run to completion that is waiting in `hang`
+ a failed assert in `pi_abort` attempting to flash a non-existent red LED

#### 5b) Debug bogus_strlen_calls

Both `strlen` and `strcpy` have been shown to work correctly for valid calls. We are now going to do a little exploration into what happens 
for calls that misuse the functions.

Review the code in the aptly-named `bogus_strlen_calls` function. 
Get together with your tablemates and look at the three "bogus" calls.
For each consider __why__ it is invalid: what is improper about the C-string that is being passed as the argument?

The string functions in the standard C library are generally not robust against bad calls, especially those that ask a function to read or write invalid memory.  The reason for this is not for performance or
due to laziness -- it's actually not possible for `strlen` to verify that its argument is a valid C-string. A `char*` is
not necessarily a pointer to a string -- it could be just a pointer
to a single character. Furthermore the address might not have a char pointee  at all -- it could be an int stored there or the address might be completely invalid or contents uninitialized. The C language does not have a reliable means to determine the validity of a pointer and reject the bad ones.

Uncomment the call to `stress_test_strlen` in `main()`. Rebuild the program and run it under gdb. Single step through the call to
`bogus_strlen_calls` and print the value returned from each of the
bad calls. Is the result what you anticipated?  What did you learn from this about the
observed consequences of reading uninitialized or invalid memory? Confirm you understanding with this check-in question[^3]

#### 5c) The dangers of C-strings

The fact that a C-string depends on a properly-placed NULL terminator to mark the end is tremendously unsafe. If you forget to write the terminator or forgot to account for that extra byte when allocating space, it can be easy to scribble over memory. For example, consider this code:

```c
struct request {
  char* filename;
  char* host;
};

void process_request(struct request* request) {
  char filename[128];
  strcpy(filename, request->filename);
  read_file(filename); 
}
```

This code might seem contrived, but it's similar to what a lot of web server code
looked like in the early 2000s. Recall from lecture that variables allocated inside
functions are put on the stack: `filename` is an array of 128 bytes on the stack. 

Talk with your neighbor: what happens if `request->filename` is longer than 128 characters?
What will the call to `strcpy` do?
How might this cause software to fail or act unpredictably? If this code existed in a device
such as the Therac-25, could it cause a serious failure? It's useful to note that that
[the git codebase (source code for the git program itself) bans the use of `strcpy`](https://github.com/git/git/blob/master/banned.h). Instead, you always are supposed to use `strncpy`, which has an explicit maximum
length for copying. E.g., this code would be OK:

```c
struct request {
  char *filename;
  char *host;
};

void process_request(struct request *request) {
  char filename[128];
  // 127 to make sure there's space for a \0
  stnrcpy(filename, request->filename, 127);
  read_file(filename); 
}
```

#### 5d) Differences under simulation
It is important to be aware of the discrepancies you may observe when comparing the behavior of a program running on the Pi versus running under the gdb simulator. Read the section titled [Differences due to simulation](/guides/gdb/#differences-due-to-simulation) in our gdb guide to be introduced to some of the issues you may run into.

Change to the directory `lab3/simulator` directory and review the program in the `buggy.c` file  Trace through the operation of the program. What do you predict will be printed as output?

Build the program using `make`. You should get warnings from the compiler about the use of uninitialized variables.

Use `make run` to run the program on the Raspberry Pi. Does the output printed by the program match what you predicted earlier?  Use `make run` to run the program again. Is the output printed the same as the previous run?

Now load the same program into gdb simulator `arm-none-eabi-gdb buggy.elf`. If you run this program on the simulator, the program does not have the same output that you observed when running on the Pi. Why is it different?

Type `Control-c` to stop the program. Without exiting gdb, use `run` to run the program for a second time. How does this output compare to the previous run? Re-run the program a few more times in gdb until you understand the pattern. What have you learned about how the simulator handles the state of memory between runs? How does this compare to what happens to the state of memory when you reset the actual Pi and re-run the program?

The gdb simulator is a powerful addition to your toolbox, but it is important to understand its limitations and differences from an actual Pi. [^4]



## Check in with TA

Answer these questions that probe on topics from the lab and review your answers with the TA to confirm your understanding.

[^1]: What `gdb` commands can you use to interrupt an program and figure out where the program is executing and view current program state (values of registers, parameters, variables, etc.)?   

[^2]: Why is it necessary to plug in both `TX` and `RX` for the echo program to work correctly?

[^3]: On a hosted system, executing an incorrect call to `strlen` (e.g. argument is an invalid address or unterminated string) can result in a runtime error (crash/halt). But when running bare metal on the Pi, every call to `strlen` (whether well-formed or not) will complete "successfully" and return a value. Explain the difference in behavior. What will be the return value for an erroneous call?

[^4]: Running a program under the gdb simulator is not be a perfect match to running on the actual Pi. A particularly frustrating situation can come from a program that exhibits a bug running on the Pi, but appears to run correctly under the simulator or work on a first run in gdb but not a subsequent. What underlying difference(s) between the simulator and real thing might contribute to this kind of inconsistency?

