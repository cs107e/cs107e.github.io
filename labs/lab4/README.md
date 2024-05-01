---
released: true
permalink: /labs/lab4/
title: 'Lab 4: Linked and Loaded'
attribution: Lab written by Pat Hanrahan and Julie Zelenski

toc: true
readings: |
  [Prelab](/labs/lab4/#prelab-preparation) to prep before lab.
---
{% comment %}
Task list to copy/paste when creating PR for this lab:

__Before releasing lab4:__
- [ ] Review writeup/code/checkin questions (instructor)
- [ ] Walk through (SL)
- [ ] Followup on issues from previous quarter postmortem (issue #394)

__To prep for lab4:__
- [ ] Make copies memory diagrams (ideally in color if possible) to hand out in lab

{% endcomment %}

## Goals

During this lab you will:

- Understand how executing programs are laid out in memory, 
  including the organization of stack frames.
- Explore the stack and heap in preparation for assignment 4.
- Experiment with the linker `ld`, with the goal of understanding what is in
  an object (ELF) file by using `nm`, and how object files are combined to produce an executable.
- Diagnose and resolve common build failures.

## Prelab preparation
To prepare for lab, do the following: 

- Review the [recap of stack frames](/lectures/Memory/stack_notes) in readings for Monday's lecture.


## Lab exercises
<a name="stack"></a>

### 0. Pull lab starter code

Change to your local `mycode` repo and pull in the lab starter code:

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull code-mirror lab4-starter
```

### 1. Stack

Change to the directory `lab4/stack`. You are going to trace the execution of a small program in the `example.c` file. Have a printed copy of the program's [starting memory diagram](images/stopped-main.html) ready for making notes.

Build the program using `make debug` and use `start` to begin program under the simulator and stop at `main`, poke around to make observations of the current state.

```console?prompt=(gdb)
Temporary breakpoint 5, main () at example.c:32
5   int main(void) {
(gdb) x/10i 0x40000000
(gdb) x/12i main
(gdb) print $pc
(gdb) x/4wx $sp
```

The [starting memory diagram](images/stopped-main.html) shows the state of memory up to the point where `main` was called. What is shown in the diagram should match the observations you make in gdb. Here are some key details to take a look at:
- Look at the memory diagram and find addresses `0x40000000` to `0x400001e8`. This is the "text" section, containing binary-encoded instructions. The instructions for main begin at address `0x400001bc`.
- The gdb `x` command ("examine memory") can be used to display contents of memory as though it were an array.
    - Use `help x` to learn more about the options for repeat count, format, and size.
    - Try `x/10i 0x40000000`. This command prints 10 instructions (`10i`) starting at address `0x40000000`.
    - You can also use the name of a symbol in place of an address, such as `x/12i main`. Confirm that the instructions for `main` match the contents shown in the memory diagram.
- `print $pc` to see value of `pc` register. Confirm that `pc` corresponds to the first instruction of `main`. This is where the program is currently stopped.
- The stack pointer was initialized to address `0x50000000` and grows downward as new values are pushed.  Where the program is stopped, the stack contains just one stack frame for the function `_cstart`. That frame is size 16 bytes and occupies locations `0x4ffffff0`- `0x4ffffffc` on the stack.
    - Try examine `x/4wx $sp` to display four words in hex (`4wx`) starting from address `sp`. ("word" is 32-bits) This command shows the topmost four words on the stack -- neat!
    - This should match what is shown in the memory diagram for the stack frame.

#### gdb commands to single step by assembly instruction
You have previously used the gdb `step` command to execute a single C statement, today you'll be using the low-level `stepi` command to execute a single assembly instruction.
The gdb `display` command can be used to set up an expression to be auto-printed after each command. One handy command to try right now is `display/i $pc` to auto-print the instruction at `$pc`. Thus, as you single-step, gdb will auto-print the next instruction to be executed.

```console?prompt=(gdb)
(gdb) display/i $pc
=> 0x400001bc <main>:   add sp,sp,-16
```

#### Trace function prolog
Use `disassemble main` to view its assembly instructions:

```console?prompt=(gdb)
(gdb) disassemble main
Dump of assembler code for function main:
=> 0x00000000400001bc <+0>: add sp,sp,-16
   0x00000000400001c0 <+4>: sd  ra,8(sp)
   0x00000000400001c4 <+8>: sd  s0,0(sp)
   0x00000000400001c8 <+12>:    add s0,sp,16
   0x00000000400001cc <+16>:    li  a1,7
   0x00000000400001d0 <+20>:    li  a0,5
   0x00000000400001d4 <+24>:    jal 0x40000104 <combine>
   0x00000000400001d8 <+28>:    jal 0x40000170 <follow_me>
   0x00000000400001dc <+32>:    ld  ra,8(sp)
   0x00000000400001e0 <+36>:    ld  s0,0(sp)
   0x00000000400001e4 <+40>:    add sp,sp,16
   0x00000000400001e8 <+44>:    ret
End of assembler dump.

```
The first few instructions of a function set up the stack frame. These instructions are called the function _prolog_.  A reciprocal set of instructions at the end of the function (the function _epilog_) tear down the frame. The prolog and epilog of every function are similar in spirit, but the specifics differ somewhat based on how many registers are saved and the space needed for local variables. The instructions in the middle are the _body_ of the function and they differ based on the function's specific operation.

We want you to carefully trace the instructions in the function prolog to understand how the stack frame is set up. Have your printed memory diagram on hand and as you trace, manually update the diagram to add the stack frame for `main`. As you work it out by hand, you can also gdb to single-step and print/examine updated values of registers/memory and confirm that values match your expectation. Ask questions and discuss with your labmates to help one another get the full picture.

Here is a walkthrough of the prolog instructions:

1.  `add sp,sp,-16`
    - This first instruction adjusts the stack pointer down by 16 bytes. This is setting aside 16 bytes of space on the stack to be used as the stack frame for the function `main`.
    - The RISC-V ABI requires that the stack pointer always be aligned to a multiple of 16, so sometimes the adjustment includes some padding (e.g. say only 24 bytes were needed, it would adjust the stack pointer by 32 to meet the alignment rule).
2. `sd  ra,8(sp)` and `sd fp/s0,0(sp)`
    - The next two instructions save the values of the `ra` and `fp` registers to the stack.
    - An `sd` instruction reads the full 64-bit register (`d` = "double-word") and stores to the destination address.
    - Note that `fp` is the symbolic name for register `s0`. Although gdb accepts `$fp` as a synomyn for `$s0`, it always refers to the register as `s0`. In this writeup, we refer to the register as `fp/s0` to keep two names connected in your mind.
    -  Every RISC-V stack frame starts with these same two saved registers. The first register stored to the stack frame is always `ra` and underneath it is `fp/s0`
        - `saved ra`
            - What is the `ra` register used for? What will be the value of the `ra` register when starting the function `main`?
           - The `ra` is an address in the text segment.  In your memory diagram, write in that value and draw an arrow that points to that address. Which function is this address contained in?  What is the meaning of that value relative to the currently executing function?
           - Why might it be useful for the frame to store a saved copy of `ra`? What does it tell you about where the function was called from?
        - `saved fp/s0`
            - The `fp/s0` is an address in the stack. Where in the stack does this address point to?
            - Why might it be useful for the frame to contain a saved copy of `fp/s0``? How can this be used as part of a backtrace operation?
3. `add s0,sp,16`
    - The final instruction of the prolog sets `fp/s0` to "anchor" the frame.
    - After executing this instruction, where does `fp/s0` point? Relative to this anchor, at what offset can you find the saved ra? the saved fp/s0?

The next few instructions of `main` set the values for the parameters and make a call to the function `combine`. Single-step to enter the `combine` function. Use `disassemble combine` to view its assembly instruction and note that it starts with the same function prolog. Trace/single-step through the prolog, and update memory diagram to add the stack frame for `combine`.

Having traced the frame setup twice, hopefully you are starting to get a feel for how the stack operates. Each function has same starting sequence: adjust the stack pointer to make space, store the saved `ra` and `fp/s0` to the stack, then anchor the `fp/s0`.

Compare your memory diagram with your tablemates. Ask questions of each other and resolve any discrepancies. We want everyone to have a rock solid undertanding of how a stack frame is laid out and what each of the values mean. Check in with us to confirm your understanding. [^1]

#### Function epilog
The last four instructions of `combine` are the function _epilog_. These instructions tear down the stack frame and restore caller-owned registers before returning to the caller. The epilog simply reverses of the operations done in the prolog, i.e. restores the registers to the value they had at funtion entry and returns stack returned to starting state.

Here is a walkthrough of the epilog instructions of `combine`:
1. `ld ra,8(sp)` and `ld s0,0(sp)`
    - This restores the `ra` and `fp/s0` registers to the values they had on entry to the function.
2. `add sp,sp,16`
    -  This adjusts `sp` to remove the frame from the stack.
3. `ret`
    - Return control to the caller and resume execution at the instruction at `ra` address.


#### Stack frame local variables
Let's look at one other function, `make_array`. This function declares an array local variable. This array will be stored in the function's stack frame.

```c
int make_array(void) {
    int array[6];
    array[2] = 9;
    array[0] = 13;
    return sum(array, 6);
}
```
```
Dump of assembler code for function make_array:
   0x0000000040000134 <+0>: add sp,sp,-48
   0x0000000040000138 <+4>: sd  ra,40(sp)
   0x000000004000013c <+8>: sd  s0,32(sp)
   0x0000000040000140 <+12>:    add s0,sp,48
   0x0000000040000144 <+16>:    li  a5,9
   0x0000000040000148 <+20>:    sw  a5,-32(s0)
   0x000000004000014c <+24>:    li  a5,13
   0x0000000040000150 <+28>:    sw  a5,-40(s0)
   0x0000000040000154 <+32>:    li  a1,6
   0x0000000040000158 <+36>:    add a0,s0,-40
   0x000000004000015c <+40>:    jal 0x400000c4 <sum>
   0x0000000040000160 <+44>:    ld  ra,40(sp)
   0x0000000040000164 <+48>:    ld  s0,32(sp)
   0x0000000040000168 <+52>:    add sp,sp,48
   0x000000004000016c <+56>:    ret
End of assembler dump.
```

Look at the assembly for `make_array` and do a quick sketch of its stack frame.

- How much additional space is allocated on the stack for the array local variable?
- How are the contents of the array initialized (or not)?
- If the code erroneously wrote to an index past the end of this stack-allocated array, what data would be corrupted?  What kind of consequence might result from this error?
- If the code erroneously returned the address of a stack-allocated array, what might happen when the caller try to access the memory at that address?

Writing outside the bounds of a stack-allocated array can be a devastating error.  The array is located in stack memory nearby to the saved values for key registers such as `fp/s0` and `ra`.  Writing past the array can overwrite these precious saved values which then causes execution to "teleport" to a completely nonsensical place. Even writing a single extra byte is too far when the array immediately abuts a critical saved value. Ouch! Stack corruption can be an utterly mystifying experience the first time you encounter it. Knowing how the stack housekeeping is managed should give you helpful insights into recognizing the problem and debugging it.

#### Solving a stack mystery

Your newfound understanding of how the stack is managed is a neat superpower that you can now test out. Here is a [mystery diagram](images/mystery.html) of the example program stopped somewhere else in its execution. In this diagram, the stack is not annotated with labels and frame divisions as before. The stack memory looks to be a inscrutable jumble of hex numbers.  However, the current value of `fp/s0` and `sp` are marked. How can you use these anchors to get a foothold to the stack frame of the currently executing function? How can you then work backwards from there to the frame of the caller and its caller and so on? Manually unwind and annotate the stack memory with labels to identify each stack frame and its contents. You have just produced your first backtrace!

### 2. Heap

Change to the directory `lab4/heapclient` to begin your foray in heap allocation.
So far we have stored our data either as local variables on the stack or global variables in the data segment. The functions `malloc` and `free`  offer another option, this one with more precise control of the size and lifetime and greater versatility at runtime.

Study the program `heapclient.c`. The `tokenize` function is used to 
dissect a string into a sequence of space-separated tokens. The function calls on the not-yet-implemented function `char *strndup(const char *src, size_t n)` to make a copy of each token. The intended behavior of `strndup` is to return a new string containing the first `n` characters of the `src` string.

Talk over with your partner why it would not be correct for `strndup` to declare a local array variable in its stack frame to store the new string.  When a function exits, its stack frame is deallocated and the memory is recycled for use by the next function call.  What would be the consequence if `strndup` mistakenly returns a pointer to memory contained within its to-be-deallocated stack frame?

Instead `strndup` must allocate space from the heap, so that the data can persist after the function exits. Edit `strndup` to use a call to `malloc` to request the necessary number of bytes. How many total bytes of space are needed to store a string with `n` characters?  

Now that you have the necessary memory set aside, what function from the `strings` module can you call to copy the first `n` characters from the `src` string to the new memory?

What is the final step you must take to complete the new string? (Hint: how is the end of a string marked?)

Once you have completed your implementation of `strndup` to make a proper heap copy of the string, build and run the program to verify your code is correct.

Unlike stack and global memory, which is automatically deallocated on your behalf, you must explicitly free dynamic memory when you are done with it. For the finishing touch, edit `main` to add the necessary calls to `free` to properly deallocate all of the heap memory it used. You're ready for this check-in question.[^2]

### 3. Linking and memmap

In this exercise, you will repeat some of the live coding demonstrations shown in the lecture on linking and loading.

Let's first review some terminology. An _object file_ (also called an .o file or a relocatable) is the result of compiling and assembling a single source file. An object file is on its way to becoming a runnable program, but it's not finished. The linker takes over from there to combine the object file with  additional object files and libraries. The linker is responsible for resolving inter-module references and relocating symbols to their final location. The output of the linker is an _executable_ file, this represents a full program that is ready to run.

#### Symbols in object files

Change to the `linking` directory of `lab4`. Read over the code in the files `start.s` and `cstart.c` and then build the object files `start.o` and `cstart.o`:

```console
$ make start.o cstart.o
```
The tool `nm` lists the symbols in an object file. Each function, variable, and constant declared at the top-level in the module is a _symbol_. Try `nm` out now:

```console
$ riscv64-unknown-elf-nm -n start.o cstart.o
```

What symbols are listed for `start.o`? For `cstart.o`? How do the symbols listed correspond to the functions defined in the source files?
What is the significance of the number shown in the left column for each symbol?
What do each of the single letters `T`, `U`, and `t` in the second column mean? 

Skim the man page to learn a little bit about this tool and the variety of symbol types:
```console
$ man riscv64-unknown-elf-nm
```
Our modules will typically contain text (code) symbols and data symbols (with variants common, uninitialized, read-only). What is the significance of upper versus lowercase for the symbol type? What does the `-n` flag do? What about the `-l` flag? Try adding it to your call to `riscv64-unknown-elf-nm` and see what happens.

Let's look at the symbols in a more complex object file. Review the variable definitions in the source file `linking.c`. Build `linking.o` and view its symbol list:
```console
$ make linking.o
$ riscv64-unknown-elf-nm -n linking.o
```
How many symbols are listed for `linking.o`?  What do the single letter symbols `D`, `R`, and `b` mean in the `nm` output?

Can you match each function/variable definition in `linking.c` to its symbol in the `nm` output? A few of the variables defined seem to have been completely optimized out, what made that possible? None of the parameters or stack-local variables in `linking.c` are listed as symbols, why not?

#### Symbols in an executable

After compiling each individual source file into an object file, the final build step is to link the object files and libraries into a program executable. The three object files we examined above are linked together in `linking.elf`.  Use `make linking.elf` to perform the link step and then use `nm` to look at the symbols in the final executable.

```console
$ make linking.elf
$ riscv64-unknown-elf-nm -n linking.elf
```
The executable contains the union of the symbols in the three object files. What is the order of the symbols in the executable? How have the symbol addresses changed during the link process?
Do any undefined symbols remain?


The compiler processes only a single module (file) at a time and thus it can only resolve references to symbols that appear within the module currently being compiled. The linker runs in a subsequent pass to perform tasks that require joining across modules. The process of filling in the missing placeholder addresses with the final symbol locations is known as _resolution_.

The linker is given a list of object files to process and it will combine the files together and arrange symbols into their final locations (relocation) and resolve cross-module references (resolution).

Answer the check-in question on linking.[^3]

#### Memmap

As part of the relocation process, the linker places all of the symbols into their final location. You supply a _memory map_ to the linker to indicate the layout of the sections. Let's look into this file to better understand its purpose and function.

Change to the `lab4/linking` directory and use `nm` to see the final locations of all the symbols in the executable.

```console
$ riscv64-unknown-elf-nm -n linking.elf
```

Note how all symbols of a given type (text, data, rodata, etc.) are grouped together into one section.

Now open the file `memmap.ld` in your text editor. `memmap.ld` is a _linker script_, which tells the linker how to lay out the sections in the final executable file.

- Do you see how the `memmap.d` linker script specifies how sections from
  individual files are to be combined?

- One of the purposes of `memmap.ld` is to ensure that the symbol `_start`
  appears first in the executable file. Why is it critical that this 
  function be first? How does `memmap.ld` specify where `_start`
  should be placed?

- Another purpose of `memmap.ld` is to setup block storage of uninitialized
  variables. Look for the symbols `__bss_start` and
  `__bss_end`. Where are those symbols placed? In a C program, uninitialized global
  variables are to be set to 0 at program start. How does `_cstart` use the bss start/end symbols to
  zero the unset variables?

Our projects use our standard `memmap.ld`, which defines the layout for a standard bare-metal C program for the Mango Pi. You are unlikely to need to edit or customize it.  However, if you curious to learn more, see the [documentation on linker scripts](https://sourceware.org/binutils/docs-2.21/ld/Scripts.html).

### 4. Diagnosing build errors

A benefit to learning the tools and steps of the build process is that this understanding gives you the needed insight to diagnose and resolve build errors.

Change to the `build_errors` directory of `lab4`. There are four subfolders, each containing a small project that has a build error. C

Change to each directory and try to build the project and see if you can diagnose the error and fix the project so it builds.
- Which tool is reporting a problem? (preprocessor, compiler, assembler, linker, make, ...)
- What is the warning/error message? What is this message trying to communicate?
- What can you change that will correct the problem?


You're ready for the final check-in question.[^4]

## Check in with TA
<a name="checkin"></a>
Each question below is a diagnostic question for a lab section. Test your understanding by answering the corresponding question as you finish an exercise. We encourage you to check in with a TA after
completing a section or two, while the material is still fresh in your mind.

[^1]: Show us your completed memory diagram for the example program stopped in `combine` (or even better, the annotated mystery diagram if you deciphered it). Here is our copy of [stopped in combine](images/stopped-combine.html) and [mystery deciphered](images/mystery-deciphered.html) if you'd like to compare your answers.
[^2]: How should the number of `free` calls relate to the number of `malloc` calls in a correctly-written program that uses heap allocation?
[^3]: Compare the symbol list from `nm linking.o` to `nm linking.elf`. How do the symbol addresses from an object file differ from the addresses in the linked executable?  How does the instruction `jal _cstart` change after linking?
[^4]: Explain the difference between an error from forgetting to `#include "module.h"` versus forgetting to link against `module.o`.
