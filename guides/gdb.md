---
title: Guide to using GDB in simulation mode
toc: true
---

The GDB debugger is a superb tool for observing and manipulating a
running program. Becoming facile with a full-featured debugger such as
`gdb` adds a critical superpower to your effectiveness as a software engineer. 

In our bare metal world, the options for debugging are more
limited than they would be in a hosted environment. We are not able to
monitor and interact with our program actually executing on the
Raspberry Pi. However, we can run `gdb`  on a local system in simulation mode. 
This allows you to single-step through the program, set breakpoints, print variables, and
so on, all while the program is running on the simulator. Observing what 
is happening in the simulator can help you understand what your 
code is doing and help you find your bugs.


### Sample debug session
Let's try out the gdb simulator on the following program that blinks an LED.

    .global _start
    .equ DELAY, 0x3F0000

    _start: 
        ldr r0, FSEL2    // configure GPIO 20 for output
        mov r1, #1
        str r1, [r0]

        // set bit 20
        mov r1, #(1<<20)

        loop: 

            // set GPIO 20 high
            ldr r0, SET0
            str r1, [r0] 

            // delay
            mov r2, #DELAY
            wait1:
                subs r2, #1
                bne wait1

            // set GPIO 20 low
            ldr r0, CLR0
            str r1, [r0] 

            // delay
            mov r2, #DELAY
            wait2:
                subs r2, #1
                bne wait2

        b loop

We compile the program using the ARM assembler. 
The option `-g` assembles the program with debugging information. 
The assembler will place symbol information 
and line numbers in the `.o` file so that `gdb` can use it.

    $ arm-none-eabi-as -g blink.s -o blink.o
    $ arm-none-eabi-gcc -nostdlib blink.o -o blink.elf

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
    Reading symbols from blink.o...(no debugging symbols found)...done.
    (gdb)

In gdb, we must first connect to the simulator:

    (gdb) target sim
    Connected to the simulator

And then load the program:

    (gdb) load
    Loading section .text, size 0x58 vma 0x8000
    Start address 0x8000
    Transfer rate: 704 bits in <1 sec.

`gdb` prints the size of the program (58 bytes) and
the start address (0x8000).

Let's set a breakpoint at the label `_start`:

    (gdb) break _start
    Breakpoint 1 at 0x8000: file blink.s, line 6.

Note that `gdb` knows about the source file and line numbers.

    (gdb) list
    1   .global _start
    2   .equ DELAY, 0x3F0000
    3   
    4   _start:
    5       ldr r0, FSEL2 // configure GPIO 20 for output
    6       mov r1, #1
    7       str r1, [r0]
    8   
    9       // set bit 20
    10      mov r1, #(1<<20)

The `run` command starts executing the program in the simulator. It will quickly hit the breakpoint we set:

    (gdb) run
    Starting program: blink.elf
    Breakpoint 1, _start () at blink.s:6
    6   mov r1, #1

The program is stopped at line 6. It has just completed the `ldr` instruction. The next instruction is a `mov`.

Let's inspect all current register values using the command `info registers`:

     (gdb) info registers
     r0             0x20200008  538968072
     r1             0x0 0
     r2             0x0 0
     r3             0x0 0
     r4             0x0 0
     r5             0x0 0
     r6             0x0 0
     r7             0x0 0
     r8             0x0 0
     r9             0x0 0
     r10            0x0 0
     r11            0x0 0
     r12            0x0 0
     sp             0x800   0x800
     lr             0x0 0
     pc             0x8004  0x8004 <_start+4>
     cpsr           0x13    19

Or the gdb `print` command can be used to see a single register value by name. `print/x` says to print in hexadecimal format.

    (gdb) print/x $r0
    $1 = 0x20200008

Success, we loaded 0x20200008 into register 0!

Now let's step one instruction.

     (gdb) stepi

After having executed the `mov`, print the affected register to see its updated value:

    (gdb) print $r1

Again, success. We loaded 1 into r1.

The next instruction to be executed is the store instruction.
Let's execute it.

    (gdb) stepi

Let's *examine* (using the `x` command) memory at a given address
to see if it has the expected contents.

    (gdb) x 0x20200008
    0x20200008: 0x00000001
    (gdb) x $r0
    0x20200008: 0x00000001

We have stored 1 at address 0x20200008. 

Continue running this program under the debugger to see what you can observe about the program state. Step through a few interactions of the delay loop and print the values of the registers `r2` and `cpsr`.

What does CPSR stand for?
What value does `$cpsr` have when `bne` branches back to `wait1`?
What is the value when `bne` does not branch
and the loop is exited?

### Differences due to simulation 

It's important to note that running under the simulator is not the same as running on the actual Raspberry Pi. 
The simulator does not model 
any of the peripherals such as GPIO or timer. For example,
consider a clock program that drives the GPIO pins to light the 7-segment display. If you run that program in the
simulator, nothing happens. The simulator is not talking to your
Raspberry Pi (you won't even need your Pi to be plugged in), nor is the simulator doing
anything special when your program accesses the memory locations for the memory-mapped peripherals. You can step through the clock program under the simulator and examine the state, but the code that attempts to control the peripherals is non-functional -- no LED will light, the timer does not increment.

Another important issue to be aware of is that the default state of registers and memory may be different under the simulator. For a correctly written program, this difference is of no consequence, but a buggy program can be sensitive to it; in particular, those programs that mistakenly access memory out of bounds or use an uninitialized variable. The irony is that such buggy programs are exactly the ones that you will need the debugger's help to resolve, yet, frustratingly, these programs can exhibit different behavior when run under the simulator than when running on the Pi. If running in the gdb simulator, the contents of not-yet-accessed memory defaults to zero. If running the program on the actual Raspberry Pi, the contents of unaccessed memory is unpredictable. Consider a program containing the following (buggy) function:

    int gauss(int n)
    {
        int result;             // oops! never initialized
        for (int i = 1; i <= n; i++)
            result += i;
        return result;
    }

`result` is used uninitialized. If the stack variable is stored in a region of memory as-yet-untouched, its initial contents are garbage. When running on the Pi, you observe the value returned by the function to be unpredictable due to the missing initialization. 

You now try running the same program under the gdb simulator and the first call to the function returns the correct answer because the intended initialization to zero is being supplied by the simulator. The memory contents in the simulator are retained between runs, so if you run the program again without having left the simulator, this time `result` is "initialized" to its last value and function now returns an incorrect result.

Arg! These conflicting observations can be mystifying and you may start to think you are losing your mind. The difference in environment is changing the observed effect of the bug. In every case, the program is buggy; the fact that it surfaces differently is further evidence of its errant ways. Once you correct the flaw, it should run correctly in all contexts, but it may take some effect to tease out the root cause with all these confounding effects.

Despite these limitations, gdb simulation mode can be a powerful ally when you are dealing with difficult bugs. Learn to make good use of this tool, but do stay mindful of the ways in which it is not an exact match to running on the actual Raspberry Pi.

### Common commands

Here is a list of useful `gdb` commands. Many command names can be abbreviated. For example, `info registers` can be invoked as `i r`, `stepi` can be invoked `si`, `print/x` is shortened to `p/x`, and so on.

|__Command__&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|__Use to__|
|`run`|start program executing from beginning
|`Control-C`| stop the executing program, returns control to gdb
|`break [where]`|set breakpoint to stop at, [where] can be function name/line number/address of instruction
|`continue`|resume program execution
|`info break`|list all breakpoints
|`delete [n]`|remove n'th breakpoint, with no argument removes all breakpoints
|`stepi [n]`|execute next n assembly instructions, with no arg stepi 1
|`step [n]`|execute the next n lines of C source, with no arg step 1
|`next [n]`|execute the next n lines of C source, stepping over function calls
|`where`|show stack backtrace up to current execution
|`disassemble [what]`|disassemble instructions, [what] can be function name or address, if no arg disassemble currently executing function
|`info registers`|show contents of all registers
|`print/d [expr]`|print expression in decimal
|`print/x [expr]`|print expression in hex
|`print/t [expr]`|print expression in binary
|`x/HOW [addr]`|examine contents of memory in given format, HOW is 3 letters for repeatcount, format, size
|`display [expr]`|automatically print the expression each time the program is stopped
|`info display`|show all auto-display expressions
|`undisplay [n]`|remove n'th auto-display expression
|`help [cmdname]`| show help for gdb command by name
|⬆️⬇️|scroll back/forward through previously executed commands
|`quit`|quit gdb

### Configure using .gdbinit files

GDB can ask a lot of annoying questions and does not track previous commands
by default.  I put the following in a `.gdbinit` configuration file 
in my home directory to fix that:

	% cat ~/.gdbinit 
	set confirm off
    set history expansion

Whenever you start gdb, the commands from your `~/.gdbinit` configuration 
file are read and executed. These user defaults are useful for any
run of gdb.  

For a program that runs only in simulation mode, you might want to
avoid having to repeatedly set the target and load the program each time you use gdb. For this, 
add a `.gdbinit` configuration file in the project directory:

	% cat ./.gdbinit
	target sim
	load

Whenever you start gdb in this directory, the commands in the local
`./.gdbinit` file will set the simulator target and load the program
automatically.

### Tui windowing mode

By default, gdb operates in plain-text mode with a single command
window. There is also a simple graphical mode, enabled with the `-tui`
option, that can split your window into various panes:

    $ arm-none-eabi-gdb -tui blink.elf

The gdb command `layout asm` followed by `layout reg` will display the following split window. 

<img title="Tui split-screen" src="../images/tui.png">

The upper pane displays current values for all registers, the middle pane is the assembly instructions, the bottom pane is a normal gdb command window. As you single-step with `si`, the register values will update automatically (those values that changed are highlighted) and middle pane will follow instruction control flow. This is a convenient view of what is happening at the machine level -- try it out!

### Going further

- CS107's [guide to gdb](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/guide/gdb.html) is a good introduction.
- Watch Chris Gregg's [video tutorial on gdb](https://www.youtube.com/watch?v=uhIt8YqtmuQ&feature=youtu.be).
- More info on `tui` mode can be found in this [external gdb guide](https://beej.us/guide/bggdb/#regasm).
-  Looking to learn some fancier tricks? See these articles Julie wrote for a 
programming journal: [Breakpoint Tricks](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/resources/gdb_coredump1.pdf) 
and [gdb's Greatest Hits](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/resources/gdb_coredump2.pdf). 
- Last but not least, the full online gdb manual tells all: 
[http://sourceware.org/gdb/current/onlinedocs/gdb/index.html](http://sourceware.org/gdb/current/onlinedocs/gdb/index.html).

