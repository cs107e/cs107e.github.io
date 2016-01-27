---
layout: page
title: Guide to using GDB in simulation mode
---

`gdb` can be run on your laptop in simulation mode. 
Single stepping through the program will help you understand
what the instructions do.

`gdb` can also help you debug your program.
Be aware, however, that simulation is not the same
as running the real program.
The simulator does not know about the peripheral
registers such as GPIO on the Raspberry Pi.
Not only doesn't it know about the registers,
but it doesn't know what the registers do.
For example, if you turn on an LED in simulation mode, nothing happens.
There are definitely things you can not debug in simulation mode.

For example, suppose we want to simulate the following
program to blink an LED on and off.

    .globl _start
    _start:

    // configure GPIO 20 for output
    ldr r0, =0x20200008
    mov r1, #1
    str r1, [r0]

    mov r1, #(1<<20)

    loop:

    // set GPIO 20 high
    ldr r0, =0x2020001C
    str r1, [r0]

    // delay
    mov r2, #0x3F0000
    wait1:
    subs r2, #1
    bne wait1

    // set GPIO 20 low
    ldr r0, =0x20200028
    str r1, [r0]

    // delay
    mov r2, #0x3F0000
    wait2:
    subs r2, #1
    bne wait2

    b loop

We compile the program using the ARM assembler. 
The option `-g` assembles the program
in debug mode which causes the assembler to place debugging information
such as symbol tables and line numbers in the `.o` file.

    % arm-none-eabi-as -g blink.s -o blink.o
    % arm-none-eabi-ld blink.o -o blink.exe

We can now run `blink.exe` using the simulator in `gdb`.

    % arm-none-eabi-gdb blink.exe
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

The first thing we need to do is configure `gdb` to use a simulator.

    (gdb) target sim
    Connected to the simulator

Then load the program

    (gdb) load
    Loading section .text, size 0x48 vma 0x8000
    Start address 0x8000
    Transfer rate: 576 bits in <1 sec.

`gdb` prints the size of the program (48 bytes) and
the start address (0x8000).

Let's set a breakpoint at `_start`.

    (gdb) break _start
    Breakpoint 1, _start () at blink.s:6

Note that `gdb` knows about the source file and line numbers.

    (gdb) list
    1   .global _start
    2   
    3   _start:
    4   
    5   // configure GPIO 20 for output
    6   ldr r0, =0x20200008
    7   mov r1, #1
    8   str r1, [r0]
    9   
    10  mov r1, #(1<<20)

Now run it

    (gdb) run
    Starting program: blink.exe
    Breakpoint 1, _start () at blink.s:6
    6   mov r1, #1

Note that we ran the first `ldr` instruction 
and the program stopped at line 7, the `mov` instruction.

Let's inspect all the register values

    (gdb) info registers
    r0             0x20200008 538968072
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
    sp             0x800 0x800
    lr             0x0 0
    pc             0x8004 0x8004 <_start+4>
    cpsr           0x13 19

or a single value register 0, `$r0`.

    (gdb) print/x $r0

Success, we loaded 0x20200008 into register 0!
`print/x` says to print in hexadecimal format.

Now let's step one instructionn, and inspect the registers again.

    (gdb) stepi
    8   str r1, [r0]

Again, success. We loaded 1 into r1.
The next instruction to be executed is the store instruction.
Let's execute it.

    (gdb) stepi
    9   mov r1, #1

Let's *examine* memory to see if it has the right contents.

    (gdb) x 0x20200008
    0x20200008: 0x00000001
    (gdb) x $r0
    0x20200008: 0x00000001

We have stored 1 at address 0x20200008. 

Continue debugging this program.
In particular, debug the delay loop.
What does CPSR stand for?
Inspect the values of CPSR as you step through the delay loop.
What value does CPSR have when `bne` returns to `wait1`?
What values does CPSR have when `bne` does not branch
and the loop is exited?

### Useful setup.

GDB can ask a lot of annoying questions and not recall previous commands
by default.  I put the following in a ".gdbinit" file in my home directory
to fix that:

	% cat ~/.gdbinit 
	set history expansion
	set confirm off

These defaults are useful in general.  Similarly, to avoid having to set
the target and load the program I put:

	% cat ~/.gdbinit.arm 
	target sim
	load

And set the following alias:

    $ alias rgdb="arm-none-eabi-gdb  --command=~/.gdbinit.arm"


### Commands

Here is a list of useful `gdb` commands.

|Command|Abbrv|Description|
|:------|:---:|:----------|
|run|r|start program|
|quit|q|quit gdb|
|<up-arrow>||scroll up through already executed commands (down-arrow goes down)|
|cont|c|continue execution after a break|
|break [addr]|b [addr]|set breakpoint to addr|
|delete [n]|d [n]|removes n'th breakpoint|
|delete|d|removes all breakpoints|
|info break|i break|lists all breakpoints|
|stepi|si|execute next instruction|
|stepi [n]|si [n]|execute next n instructions|
|nexti|ni|execute next instruction, stepping over function calls|
|nexti [n]|ni [n]|execute next n instructions, stepping over function calls|
|next|n|execute the next line of source (can be multiple instructions)|
|where||show where execution halted|
|disas [addr]||disassemble instructions at given address|
|disas/r [addr]||same, adds hex machine code for instructions|
|disas [b,e]||same, but disassembles the range [b,e] inclusive|
|info registers|i r|dump contents of all registers|
|print/d [expr]|p/d [expr]|print expression in decimal|
|print/x [expr]|p/x [expr]|print expression in hex|
|print/t [expr]|p/t [expr]|print expression in binary|
|x/NFU [addr]||Examine contents of memory in given format|
|display [expr]||automatically print the expression each time the program is halted|
|info display||show list of automatically displays|
|undisplay [n]||undisplay 1 remove an automatic display|


### Random examples

Most people don't know about gdb macros, but they can be nice.  Suppose we
want to see which instructions in our code modify the cpsr register.
We can define a simple macro "np" to step a single instruction and print
the cpsr register in hex.

	(gdb) def np
	Type commands for definition of "n".
	End with a line saying just "end".
	>nexti
	>p/x $cpsr
	>end

Run it a bit:

	(gdb) b _start
	Breakpoint 1 at 0x8004: file blink.s, line 6.
	(gdb) r
	6 mov r1, #1
	(gdb) np
	7	str r1, [r0]
	$1 = 0x13
	...

The value of cpsr will be printed after each executed instruction.

### Getting GDB

If you are on Linux and don't have the ARM GDB installed, you can use
this simple bash script (save it to a file, and run "bash <file>"):

	export TARGET=arm-none-eabi
	export PREFIX=$HOME/bin/$TARGET
	export PATH=$PATH:$PREFIX/bin
	export VERSION=7.8.1
	export GDB=gdb-$VERSION

	rm -rf $GDB

	# Get archives
	wget http://ftp.gnu.org/gnu/gdb/$GDB.tar.gz

	# Extract archives
	tar xzvf $GDB.tar.gz 

	mkdir build-gdb
	cd build-gdb
	../$GDB/configure --target=$TARGET --prefix=$PREFIX --enable-interwork --enable-multilib
	make 
	make install

GDB will now be in:

	%  ls ~/bin/arm-none-eabi/bin/
	arm-none-eabi-gdb*  arm-none-eabi-run*

Note that if you are missing packages this depends on, it can fail in mysterious ways.    My distribution was missing makeinfo and screen manipulation routines.
I am on Ubuntu so a package install:

	sudo apt-get install texinfo
	sudo apt-get install libncurses5-dev

fixed the problem.  There are guides for installing on Window/Mac under the "guides" section.
