---
title: "Guide: Using GDB in Simulation Mode"
attribution: Written by Julie Zelenski
---

The GDB debugger is a superb tool for observing and manipulating a
running program. Becoming facile with a full-featured debugger such as
`gdb` adds a critical superpower to your effectiveness as a software engineer. 

In our bare metal world, the debugger options are more
limited than they would be in a hosted environment. You will not be able to
monitor and interact with your program while it is actually executing on the Pi. However, you can use the debugger in simulation mode. You run `gdb` on your laptop and execute your program inside the debugger using the built-in RISCV simulator.
Under simulation, you can single-step through your code, set breakpoints, print variables, examine registers and memory, and more. These observations can help you to understand what your code is doing and diagnose bugs.  

Using the debugger is largely the same whether in simulation mode or not, but some programs may execute differently under simulation than when running on the actual Pi. Be sure to read the section below on those [differences due to simulation](#simulation).

## Sample gdb session
Let's try out the gdb simulator on the following simple `sum` program. The code is availble in the folder `$CS107E/sample_build` if you want to follow along.

```c
int sum(int n) {
    int result = 0;
    
    for (int i = 1; i <= n; i++)
        result += i;
    return result;
}

int main(void)  {
    int a = sum(10);
    int b = sum(5);
    return a + b;
}
```

The `sum.elf` file is typically the penultimate step in our build, right
before we extract the raw binary instructions into the `sum.bin` that is
sent to the Pi.  The `elf` version of the file is the one used by the gdb simulator.

Run `make debug` to start gdb on the `sum.elf` file

```console?prompt=(gdb),$
$ make debug
riscv64-unknown-elf-gdb -q --command=$CS107E/other/gdbsim.commands sum.elf
Reading symbols from sum.elf...
Auto-loading commands from $CS107E/other/gdbsim.commands...
Connected to the simulator.
Loading section .text, size 0x5a14 lma 40000000
Loading section .rodata, size 0x26b4 lma 40005a18
Loading section .eh_frame, size 0x2c lma 400080d0
Loading section .data, size 0x16e0 lma 40020000
Start address 40000010
Transfer rate: 310944 bits in <1 sec.
Breakpoint 1 at 0x40000a74
(gdb)
```

Let's review the code for `main`. Note that `gdb` knows about the source file and line numbers because our Makefile compiles the code with the debugging flag `-g`.

```console?prompt=(gdb)
(gdb) list main
4       for (int i = 1; i <= n; i++)
5           result += i;
6       return result;
7   }
8
9   int main(void)  {
10      int a = sum(10);
11      int b = sum(5);
12      return a + b;
13  }
```

Set a breakpoint on line 11 of this file:
```console?prompt=(gdb)
(gdb) break 11
Breakpoint 2 at 0x4000005c: file sum.c, line 11.
```
The `run` command starts executing the program in the simulator. It will quickly hit the breakpoint we set:

```console?prompt=(gdb)
(gdb) run
Starting program: sum.elf
Breakpoint 2, main () at sum.c:11
11      int b = sum(5);
```
The program is stopped at line 11. This is _before_ this line of C has executed.

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
12      return a + b;
(gdb) print b
$3 = 15
```

Use `continue` to let the program resume execution.  The main function will finish executing, and gdb will print a message that the process exited.

```console?prompt=(gdb)
(gdb) continue
Continuing.
[Inferior 1 (process 42000) exited normally]
```

The above information means the program exited normally. This is the expected behavior for a C program that has successfully run to completion.

<A name="simulation"></A>
## Differences due to simulation 

It's important to note that running under the simulator is not the same as running on the actual Pi.
The simulator does not model the peripherals such as GPIO or timer. For example,
the blink program we studied early in the quarter drives an LED connected to GPIO PB0. If you run this program in the
simulator, the LED will not light. The simulator is not talking to your
 Pi (you won't even need your Pi to be plugged in), nor is the simulator doing
anything special when your program accesses the memory locations for the memory-mapped peripherals. You can step through the blink program under the simulator and examine the state, but the code that attempts to control the peripherals is a no-op -- no LED will light, the timer does not increment.

Another important issue to be aware of is that the default state of registers and memory may be different under the simulator. For a correctly-written program, this difference is of no consequence, but a buggy program can be sensitive to it; such as a program that mistakenly accesses memory out of bounds or uses an uninitialized variable. The irony is that such buggy programs are exactly the ones that you will need the debugger's help to resolve, yet, frustratingly, these programs can exhibit different behavior when run under the simulator than when running on the Pi. If running in the gdb simulator, the contents of not-yet-accessed memory defaults to zero. If running the program on the actual Pi, the contents of un-accessed memory is unpredictable. For example, if the sample program we used above had a bug where it forget to initialize the result:
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

Despite its limitations, gdb simulation mode can be a powerful ally when dealing with difficult bugs. Learn to make good use of this tool, but do stay mindful of the ways in which it is not an exact match to running on the actual Pi.

> __Pro-tip__ See our [gdb quick reference](/guides/gdb-quickref) for a list of common gdb commands.
{: .callout-info}
