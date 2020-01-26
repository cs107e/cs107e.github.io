---
title: Guide to using GDB in simulation mode
toc: true
---

The GDB debugger is a superb tool for observing and manipulating a
running program. Becoming facile with a full-featured debugger such as
`gdb` adds a critical superpower to your effectiveness as a software engineer. 

In our bare metal world, the debugger options are more
limited than they would be in a hosted environment. You will not be able to
monitor and interact with your program while it is actually executing on the
Raspberry Pi. However, you can use the debugger in simulation mode. You run `gdb` on your laptop and execute your program inside the debugger using the built-in ARM simulator.
Under simulation, you can single-step through your code, set breakpoints, print variables, examine registers and memory, and more. These observations can help you to understand what your code is doing and diagnose bugs.  

Using the debugger is largely the same whether in simulation mode or not, but some programs may execute differently under simulation than when running on the actual Pi. Be sure to read the section below on those [differences due to simulation](#simulation).

### Sample gdb session
Let's try out the gdb simulator on the following C program that blinks an LED.

    static volatile unsigned int *FSEL2 = (unsigned int *)0x20200008;
    static volatile unsigned int *SET0  = (unsigned int *)0x2020001C;
    static volatile unsigned int *CLR0  = (unsigned int *)0x20200028;

    void main(void)
    {
        *FSEL2 = 1;

        while (1) {
            *SET0 = 1 << 20;
            for (int i = 0; i < 0x3f0000; i++) ;
            *CLR0 = 1 << 20;
            for (int i = 0; i < 0x3f0000; i++) ;
        }
    }

The `blink.elf` file is typically the penultimate step in our build, right 
before we extract the raw binary instructions into the `blink.bin` that is
sent to the Pi.  The `elf` version of the file is the one used by the gdb simulator.

Run `gdb` on the `blink.elf` file:

    $ arm-none-eabi-gdb blink.elf
    GNU gdb (GDB) 7.8.1
    Copyright (C) 2014 Free Software Foundation, Inc.
    License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
    This is free software: you are free to change and redistribute it.
    There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
    and "show warranty" for details.
    This GDB was configured as "--host=x86_64-apple-darwin14.0.0 --target=arm-none-eabi".
    Type "show configuration" for configuration details.
    For bug reporting instructions, please see:
    <http://www.gnu.org/software/gdb/bugs/>.
    Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.
    For help, type "help".
    Type "apropos word" to search for commands related to "word"...
    Reading symbols from blink.elf...done.
    (gdb)

In gdb, we first connect to the simulator:

    (gdb) target sim
    Connected to the simulator

And then load the program:

    (gdb) load
    Loading section .text, size 0xd8 vma 0x8000
    Start address 0x8000
    Transfer rate: 1728 bits in <1 sec.

`gdb` prints the size of the program (0xd8 bytes) and
the start address (0x8000).

Let's review the code for `main`. Note that `gdb` knows about the source file and line numbers because our Makefile uses compiler flag `-g`.

    (gdb) list main
    1   static volatile unsigned int *FSEL2 = (unsigned int *)0x20200008;
    2   static volatile unsigned int *SET0  = (unsigned int *)0x2020001C;
    3   static volatile unsigned int *CLR0  = (unsigned int *)0x20200028;
    4   
    5   void main(void)
    6   {
    7       *FSEL2 = 1;
    8       
    9       while (1) {
    10          *SET0 = 1 << 20;

Set a breakpoint on line 10 of this file:

    (gdb) break 10
    Breakpoint 1 at 0x8028: file blink.c, line 10.

The `run` command starts executing the program in the simulator. It will quickly hit the breakpoint we set:

    (gdb) run
    Starting program: blink.elf
    Breakpoint 1, main () at blink.c:10
    10          *SET0 = 1 << 20;

The program is stopped at line 10. This is before this line of C has executed.

The gdb `print` command can be used to view a variable or evaluate an expression.

    (gdb) print SET0
    $1 = (volatile unsigned int * const) 0x2020001c
    (gdb) print *SET0
    $2 = 0

Execute the next line.

    (gdb) next

Print the affected memory location to see its updated value. `print/x` says to print in hexadecimal format.

    (gdb) print/x *SET0
    $3 = 0x100000

Success. The assignment statement turned on bit 20!

Use `next` to execute the next line, the delay loop. The simulator operates fairly slowly, so the loop will take a while to execute. Use `Control-C` to interrupt the program and bring control back to the debugger.  Print the loop counter to see how far into the loop it is:

    ^C0x0000804c in main () at blink.c:11
    11          for (int i = 0; i < 0x3f0000; i++) 
    (gdb) print i
    $4 = 22928

Use `cont` to let the program resume execution.

<A name="simulation"></A>
### Differences due to simulation 

It's important to note that running under the simulator is not the same as running on the actual Raspberry Pi. 
The simulator does not model the peripherals such as GPIO or timer. For example,
the blink program above drives an LED connected to GPIO 20. If you run this program in the
simulator, the LED will not light. The simulator is not talking to your
Raspberry Pi (you won't even need your Pi to be plugged in), nor is the simulator doing
anything special when your program accesses the memory locations for the memory-mapped peripherals. You can step through the blink program under the simulator and examine the state, but the code that attempts to control the peripherals is a no-op -- no LED will light, the timer does not increment.

Another important issue to be aware of is that the default state of registers and memory may be different under the simulator. For a correctly-written program, this difference is of no consequence, but a buggy program can be sensitive to it; such as a program that mistakenly accesses memory out of bounds or uses an uninitialized variable. The irony is that such buggy programs are exactly the ones that you will need the debugger's help to resolve, yet, frustratingly, these programs can exhibit different behavior when run under the simulator than when running on the Pi. If running in the gdb simulator, the contents of not-yet-accessed memory defaults to zero. If running the program on the actual Raspberry Pi, the contents of unaccessed memory is unpredictable. Consider a program containing the following (buggy) function:

    int gauss(int n)
    {
        int result;             // oops! never initialized
        for (int i = 1; i <= n; i++)
            result += i;
        return result;
    }

`result` is used uninitialized. If the stack variable is stored in a region of memory as-yet-untouched, its initial contents are garbage. When running on the Pi, you observe the value returned by the function to be unpredictable due to the missing initialization. 

You now try running the same program under the gdb simulator and the first call to the function returns the correct answer because the intended initialization to zero is being supplied by the simulator. The memory contents in the simulator are retained between runs, so if you run the program again without having left the simulator, this time `result` is "initialized" to its last value and function now returns an incorrect result.

Arg! These conflicting observations can be mystifying and you may start to think you are losing your mind. The difference in environment is changing the observed effect of the bug. In every case, the program is buggy; the fact that it surfaces differently is further evidence of its errant ways. It may take some digging to sort things out, but it is a transcendent experience when you can precisely trace the cause and effect and how it resulted in the observed behaviors. Having attained a complete understanding, you can construct an appropriate fix so that the program runs correctly in all contexts.

Despite its limitations, gdb simulation mode can be a powerful ally when dealing with difficult bugs. Learn to make good use of this tool, but do stay mindful of the ways in which it is not an exact match to running on the actual Raspberry Pi.

### Common commands

Here is a list of useful `gdb` commands. Many command names can be abbreviated. For example, `info registers` can be invoked as `i r`, `stepi` can be invoked `si`, `print/x` is shortened to `p/x`, and so on.

|__Command__&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|__Use to__|
|`run`|start program executing from beginning
|`Control-C`| interrupt the executing program, give control back to gdb
|`continue`|resume program execution
|`break WHERE`|set breakpoint to stop at, `WHERE` is name of function or line number or address of instruction
|`info break`|list all breakpoints
|`delete [N]`|remove n'th breakpoint, with no argument removes all breakpoints
|`stepi`|execute next assembly instruction
|`step`|execute the next line of C source, step into function call
|`next`|execute the next line of C source, step over function calls
|`where`|show stack backtrace up to current execution
|`disassemble [WHAT]`|disassemble instructions, `WHAT` can be function name or address, if no arg disassemble currently executing function
|`info registers`|show contents of all registers
|`print/d EXPR`|eval expression, print result in decimal
|`print/x EXPR`|eval expression, print result in hex
|`print/t EXPR`|eval expression in binary, print result in binary
|`display EXPR`|auto-print expression after each command
|`info display`|list all auto-display expressions
|`undisplay [n]`|remove n'th auto-display expression
|`x/HOW ADDR`|examine contents of memory at ADDR, use format `HOW` 3 letters for repeatcount, format, size
|`help [cmdname]`| show help for gdb command `[cmdname]`
|⬆️⬇️|scroll back/forward through previously executed commands
|`quit`|quit gdb

### Configure using .gdbinit files

GDB can ask a lot of annoying questions and does not track previous commands
by default.  I put the following in a `.gdbinit` configuration file 
in my home directory to fix that:

	$ cat ~/.gdbinit 
	set confirm off
    set history expansion

Whenever you start gdb, the commands from your `~/.gdbinit` configuration 
file are read and executed. These user defaults are useful for any
run of gdb.  

For a program that runs only in simulation mode, you might want to
avoid having to repeatedly set the target and load the program each time you use gdb. For this, 
add a `.gdbinit` configuration file in the project directory:

	$ cat ./.gdbinit
	target sim
	load

Whenever you start gdb in this directory, the commands in the local
`./.gdbinit` file will set the simulator target and load the program
automatically.

### Additional resources

- CS107's [guide to gdb](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/guide/gdb.html) is a good introduction.
- Watch Chris Gregg's [video tutorial on gdb](https://www.youtube.com/watch?v=uhIt8YqtmuQ&feature=youtu.be).
-  Looking to learn some fancier tricks? See these articles Julie wrote for a 
programming journal: [Breakpoint Tricks](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/resources/gdb_coredump1.pdf) 
and [gdb's Greatest Hits](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/resources/gdb_coredump2.pdf). 
- Last but not least, the full online gdb manual tells all: 
[http://sourceware.org/gdb/current/onlinedocs/gdb/index.html](http://sourceware.org/gdb/current/onlinedocs/gdb/index.html).

