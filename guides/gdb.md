---
title: "Guide: Using GDB in Simulation Mode"
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

## Sample gdb session
Let's try out the gdb simulator on the following simple C program. This program code is available in `$CS107E/examples/gdb/program.c`.

```c
int sum(int n)
{
    int result = 0;
    
    for (int i = 1; i <= n; i++)
        result += i;
    return result;
}

int main(void) 
{
    int a = sum(10);
    int b = sum(5);
    return a + b;
}
```

The `program.elf` file is typically the penultimate step in our build, right 
before we extract the raw binary instructions into the `program.bin` that is
sent to the Pi.  The `elf` version of the file is the one used by the gdb simulator.

Run `gdb` on the `program.elf` file:

```console?prompt=(gdb),$
$ arm-none-eabi-gdb program.elf
GNU gdb (GDB) 9.2
Copyright (C) 2020 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "--host=x86_64-apple-darwin19.6.0 --target=arm-none-eabi".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from program.elf...
(gdb)
```

In gdb, we first connect to the simulator and load the program:
```console?prompt=(gdb)
(gdb) target sim
Connected to the simulator
(gdb) load
Loading section .text, size 0x5b4 lma 0x8000
Start address 0x8000
Transfer rate: 11680 bits in <1 sec.
```

Let's review the code for `main`. Note that `gdb` knows about the source file and line numbers because our Makefile compiles the code with the debugging flag `-g`.

```console?prompt=(gdb)
(gdb) list main
6           result += i;
7       return result;
8   }
9   
10  int main(void) 
11  {
12      int a = sum(10);
13      int b = sum(5);
14      return a + b;
15  }
```

Set a breakpoint on line 13 of this file:
```console?prompt=(gdb)
(gdb) break 13
Breakpoint 1 at 0x805c: file program.c, line 13.
```
The `run` command starts executing the program in the simulator. It will quickly hit the breakpoint we set:

```console?prompt=(gdb)
(gdb) run
Starting program: program.elf
Breakpoint 1, main () at program.c:13
13      int b = sum(5);
```
The program is stopped at line 13. This is _before_ this line of C has executed.

The gdb `print` command can be used to view a variable or evaluate an expression.

```console?prompt=(gdb)
(gdb) print a
$1 = 55
(gdb) print b
$2 = <optimized out>
```
The program has not yet executed the statement that assigns `b` and its value is not available. Use `next` to execute that statement. After which, you can then print `b` to see its value.

```console?prompt=(gdb)
(gdb) next
14      return a + b;
(gdb) print b
$3 = 15
```

Use `continue` to let the program resume execution.  The main function should finish executing and it will appear that the program is stuck. 

```console?prompt=(gdb)
(gdb) continue
Continuing.
```
Use `Control-C` to interrupt the running program and return control to the debugger. Use the `backtrace` command to see where the program was executing when it was interrupted:

```console?prompt=(gdb)
^C
Program received signal SIGINT, Interrupt.
0x0000800c in hang ()
(gdb) backtrace
#0  0x0000800c in hang ()
#1  0x0000800c in _start ()
Backtrace stopped: previous frame identical to this frame (corrupt stack?)
```

The above information tells you that the program is stopped in `hang` which is called from `_start`. Review the code in `start.s` and `cstart.c` to remind yourself of what happens in a C program before and after `main()`. If currently in `hang`, the program has finished and is in the final "holding pattern". This is the normal behavior for a C program that has successfully run to completion. 

<A name="simulation"></A>
## Differences due to simulation 

It's important to note that running under the simulator is not the same as running on the actual Raspberry Pi. 
The simulator does not model the peripherals such as GPIO or timer. For example,
the blink program we studied early in the quarter drives an LED connected to GPIO 20. If you run this program in the
simulator, the LED will not light. The simulator is not talking to your
Raspberry Pi (you won't even need your Pi to be plugged in), nor is the simulator doing
anything special when your program accesses the memory locations for the memory-mapped peripherals. You can step through the blink program under the simulator and examine the state, but the code that attempts to control the peripherals is a no-op -- no LED will light, the timer does not increment.

Another important issue to be aware of is that the default state of registers and memory may be different under the simulator. For a correctly-written program, this difference is of no consequence, but a buggy program can be sensitive to it; such as a program that mistakenly accesses memory out of bounds or uses an uninitialized variable. The irony is that such buggy programs are exactly the ones that you will need the debugger's help to resolve, yet, frustratingly, these programs can exhibit different behavior when run under the simulator than when running on the Pi. If running in the gdb simulator, the contents of not-yet-accessed memory defaults to zero. If running the program on the actual Raspberry Pi, the contents of un-accessed memory is unpredictable. For example, if the sample program we used above had a bug where it forget to initialize the result:
```c
int sum(int n)
{
    int result;             // oops! never initialized
    for (int i = 1; i <= n; i++)
        result += i;
    return result;
}
```

If the local variable is stored in a region of memory as-yet-untouched, its initial contents would be garbage. When running on the Pi, you observe the value returned by the function to be unpredictable due to the missing initialization. 

You now try running the same program under the gdb simulator and the first call to `sum` returns the intended result because the missing initialization to zero is being supplied by the simulator. But a subsequent call to `sum`  without having left the simulator, the `result` will now be "initialized" to its last value and function now returns an incorrect result.

Arg! These conflicting observations can be mystifying and you may start to think you are losing your mind. The difference in environment is changing the observed effect of the bug. In every case, the program is buggy; the fact that it surfaces differently is further evidence of its errant ways. It may take some digging to sort things out, but it is a transcendent experience when you can precisely trace the cause and effect and how it resulted in the observed behaviors. Having attained a complete understanding, you can construct an appropriate fix so that the program runs correctly in all contexts.

Despite its limitations, gdb simulation mode can be a powerful ally when dealing with difficult bugs. Learn to make good use of this tool, but do stay mindful of the ways in which it is not an exact match to running on the actual Raspberry Pi.

## Common commands

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
{: .table-striped .w-75 .mx-auto}


<p></p>
## Using .gdbinit configuration files

A `.gdbinit` file is used to set a startup sequence for gdb that can be used to configure the debugger or automate common actions. For example, I find it annoying that gdb asks for confirmation on many commands and does not remember command history by default.  I put the following in a `.gdbinit` configuration file 
in my home directory to fix that:

```console
$ cat ~/.gdbinit 
set confirm off
set history expansion
```

Whenever you start gdb, the commands from your user `~/.gdbinit` config 
file are read and executed. The user config file is good for any gdb commands or defaults that you want applied to every use of gdb.

<a name=autoload></a>
You can also add a local `.gdbinit` file in a project directory that applies settings specific to this project. For example, given a project that runs only in simulation mode, a local config file could automate the gdb commands to set the target and load the program. For this, 
add a `.gdbinit` file in the project directory:

```console
$ cat ./.gdbinit
target sim
load
```

Whenever you start gdb in this directory, these commands will run to set the simulator target and load the program
automatically.

<A name="faq"></A>
## Common questions

### When I start gdb, it gives a long warning about auto-loading being declined. What's wrong?
```console
warning: File ".gdbinit" auto-loading has been declined by your `auto-load safe-path' set to "$debugdir:$datadir/auto-load".
To enable execution of this file add
... blah blah blah ...
```

This indicates that your is configured to disallow auto-loading. If starting gdb in a directory containing a local `./.gdbinit` when auto-loading is disallowed, gdb prints message "auto-loading has been declined" to alert you that the config file is being ignored. Here is how to edit your user config file to allow auto-loading:

- Open your user config file `~/.gdbinit` in an editor. If file doesn't yet exist, you will need to create it. 
- Append this line verbatim:
    ```
    set auto-load safe-path /
    ```
- Save the file and exit your editor. 
- Run gdb again. The previous warning should be gone and and the local config file is now auto-loaded.

You will only need to make this edit once, gdb is now configured to always allow auto-load in future.

## Additional resources

- CS107's [guide to gdb](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/guide/gdb.html) is a good introduction.
- Watch Chris Gregg's [video tutorial on gdb](https://www.youtube.com/watch?v=uhIt8YqtmuQ&feature=youtu.be).
-  Looking to learn some fancier tricks? See these articles Julie wrote for a 
programming journal: [Breakpoint Tricks](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/resources/gdb_coredump1.pdf) 
and [gdb's Greatest Hits](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/resources/gdb_coredump2.pdf). 
- Last but not least, the full online gdb manual tells all: 
[http://sourceware.org/gdb/current/onlinedocs/gdb/index.html](http://sourceware.org/gdb/current/onlinedocs/gdb/index.html).

