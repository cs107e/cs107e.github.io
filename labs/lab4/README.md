---
released: true
permalink: /labs/lab4/
title: 'Lab 4: Linked and Loaded'
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

*Lab written by Pat Hanrahan and Julie Zelenski*

## Goals

During this lab you will:

- Understand how executing programs are laid out in memory, 
  including the organization of stack frames.
- Explore the stack and heap in preparation for assignment 4.
- Experiment with the linker `ld`, with the goal of understanding what is in
  an object (ELF) file by using `nm`, and how object files are combined to produce an executable.

## Prelab preparation
To prepare for lab, do the following: 

- Review the [recap of stack frames and APCS full frame](/lectures/Memory/stack_notes) in readings for Monday's lecture.
- Skim the code we will study for the [stack](code/stack/example.c) and [heap](code/heapclient/heapclient.c) exercises.

## Lab exercises
<a name="stack"></a>

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
$ git pull --allow-unrelated-histories starter-code lab4-starter
```

### 1. Stack

Change to the directory `lab4/stack`.  You are going to trace the execution of the small program in the `example.c` file. Have a printed copy of the program's [starting memory diagram](images/stack_example.html) ready for making notes.

Build the program using `make` and run `example.elf` under the gdb simulator. Set a breakpoint on `main`. Run the program. When stopped at the breakpoint, you will poke around to make observations of the current state.

```console?prompt=(gdb)
Breakpoint 1, main () at example.c:5
5   int main(void) {
(gdb) print $pc
(gdb) x/30i 0x8000
(gdb) x/4wx $sp
```

The [starting memory diagram](images/stack_example.html) shows the state of memory up to the point where `main` was called. What is shown in the diagram should match the observations you make in gdb. Here are some key details to study:
- In the memory diagram, addresses `0x8000` to `0x80ac` are the text section, containing binary-encoded instructions. The `pc` points to address `0x802c` which corresponds to the first instruction of `main`. The program is stopped before executing that instruction.
    - The gdb `x` command ("examine memory") can be used to display contents of memory as though it were an array.  Try `x/30i 0x8000`. This command prints 30 instructions (`30i`) starting at address `0x8000` (beginnning of text section). Confirm that instructions printed in gdb match the contents shown in the memory diagram.
    - `print $pc` to see value of `pc` register. Confirm that `pc` corresponds to the first instruction of `main` where the program is stopped.
- The stack was placed at address `0x800000` and grows downward as new values are pushed.  At this point in execution, the stack contains just one stack frame, for the function `_cstart`. That frame occupies memory addresses `0x7fffffc`- `0x7fffff0`. The `fp/r11` register points to the first value pushed in the frame (address `0x7fffffc`) and the `sp` register points to the last value pushed (address `0x7fffff0`).
    - Try examine `x/4wx $sp` to display next four words in hex (`4wx`) starting from `sp`. This shows the topmost 4 words on the stack -- neat!


#### gdb commands to single step by assembly instruction
You have previously used the gdb `step` command to execute a single C statement, today you'll be using the low-level `stepi` command to execute a single assembly instruction.
The gdb `display` command can be used to set up an expression to be auto-printed after each command. One handy command to try right now is `display/i $pc` to auto-print the instruction at `$pc` (e.g. next instruction to be executed).

```console?prompt=(gdb)
(gdb) display/i $pc
=> 0x802c <main>:   mov r12, sp
```

#### Trace function prolog
Use `disassemble main` to see its assembly instructions:

```console?prompt=(gdb)
(gdb) disassemble main
Dump of assembler code for function main:
=> 0x0000802c <+0>:     mov r12, sp
   0x00008030 <+4>:     push {r11, r12, lr, pc}
   0x00008034 <+8>:     sub r11, r12, #4, 0
   0x00008038 <+12>:    mov r1, #7, 0
   0x0000803c <+16>:    mov r0, #5, 0
   0x00008040 <+20>:    bl  0x8010 <combine>
   0x00008044 <+24>:    add r0, r0, #3, 0
   0x00008048 <+28>:    sub sp, r11, #12, 0
   0x0000804c <+32>:    ldm sp, {r11, sp, lr}
   0x00008050 <+36>:    bx  lr
End of assembler dump.
```
The first three instructions set up the APCS frame. These instructions are called the function _prolog_.  The final three instructions are the function _epilog_, which tear down the frame. The prolog and epilog are standardized across all functions. The instructions in the middle are the _body_ of the function and they differ based on the function's specific operation.

We want you to carefully trace the three instructions in the function prolog to understand how they setup the ACPS frame. Have your printed memory diagram on hand and as you trace, manually update the diagram to add the stack frame for `main`. Use gdb to single-step and print/examine updated values of registers/memory and confirm that values match your expectation. Ask questions and discuss with your labmates to help one another get the full picture.

Here is a walkthrough of the prolog instructions:

1.  `mov r12, sp`
    - This makes a copy of `sp` in `r12` in preparation for pushing the value in the second instruction.
     - This dance appears roundabout, but is unavoidable. Do you have a guess why? (Hint: trying to use value of `sp` at the same time a push instruction is changing `sp` seems problematic...)
     - What is the value of `sp` at this point in execution?
2. `push {r11, r12, lr, pc}`
    - A push instruction copies mutiple registers to the stack, processing them in order from right to left. pc is pushed first, then lr, then r12, then r11.
    - Every APCS stack frame starts with these same four saved registers.
    - `saved pc`
        - Recall that pc when instruction is executing has advanced +8 because of fetch-decode-execute pipeline. The saved pc is an address in the text segment -- which function is this address contained in?
    - `saved lr`
        - What is the value of the lr at this point? What does it represent? The saved lr is an address in the text segment. Which function is this address contained in?
    - `saved r12`
        - value in r12 is a copy of sp, think of this as representing `saved sp`
        - The saved sp is an address in the stack. Where in the stack does this address point to?
    - `saved r11`
        - `fp` is the symbolic name for register `r11`. Most tools accept either name as synonym, but in gdb, you must use the name `r11`.
        - The saved r11/fp is an address in the stack. Where in the stack does this address point to?
    - After executing the push instruction, to what location does `sp` now point?
3. `sub r11, r12, #4, 0`
    - The final instruction of the prolog sets `r11/fp` to "anchor" the frame.
    - After executing this instruction, where does `r11/fp` point?

The next few instructions of `main` set values for two parameters and make a call to the function `combine`. Single-step to enter the `combine` function. Use `disassemble combine` to view its assembly instruction and note that it starts with same standard function prolog. Trace/single-step through the prolog again, and update memory diagram to add the stack frame for `combine`.

Having traced the frame setup twice, hopefully you are starting to get a feel for how the stack operates. Each function has same starting sequence: push the same four registers and anchor the fp.

Your completed memory diagram is a roadmap to the program's use of memory and how the stack is managed. The diagram contains a lot of details, but if you take the time to closely inspect it, you will gain a more complete understanding of the relationship between the contents of memory, registers, and the executing program.  Go over it with your tablemates and ask questions of each other to help solidify your understanding. Check in with us and show off your diagram![^1]

#### Function epilog
The final three instructions of `combine` are the function _epilog_. These instructions tear down the stack frame and restore caller-owned registers before returning to the caller. You can think of the epilog simply as an abstraction that "reverses" the operations that were done in the prolog, i.e. stack returned to starting state and registers restored to values they had at function entry.

If you are curious about the details, here is a walkthrough of how the epilog instructions operate:
1. `sub sp, r11, #12, 0`
    - This resets the sp to memory address it was directly after pushing the four registers of the APCS frame.
2. `ldm sp, {r11, sp, lr}`
    -  `ldm` ("load multiple") reads a sequence of words from memory starting at a base address and copies the words into the named registers. `pop` is an alias for a `ldm` that also does an automatic adjustment the base address as a side effect (i.e. reads the word and changes the `sp` to "remove" it from the stack) .
    - This `ldm` instruction reads three words in memory starting at `sp` and copies those values into the registers `r11`, `sp`, and `lr`.  This restores those three registers to the value they had at time of function entry.
    - The prolog pushed four registers, but only three registers are being restored (all but `pc`).  Why is it important to __not__ restore the value of the `pc` to the value it had on entry?
    - How the saved registers are removed from the stack is quite subtle. Because it is not using `pop`, there is no auto adjustment of `sp`.  However, the `ldm` will load `sp` with the saved value it had at time of function entry, thus restoring `sp` to where it started and effectively removing any values that were pushed.  Tricky!
3. ` bx  lr`
    - branch exchange returns control to the caller and resumes at the instruction at the lr address.


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
0x000080a4 <+0>:    mov r12, sp
0x000080a8 <+4>:    push    {r11, r12, lr, pc}
0x000080ac <+8>:    sub r11, r12, #4, 0
0x000080b0 <+12>:   sub sp, sp, #24, 0
0x000080b4 <+16>:   mov r3, #9, 0
0x000080b8 <+20>:   str r3, [r11, #-28] ; 0xffffffe4
0x000080bc <+24>:   mov r3, #13, 0
0x000080c0 <+28>:   str r3, [r11, #-36] ; 0xffffffdc
0x000080c4 <+32>:   mov r1, #6, 0
0x000080c8 <+36>:   sub r0, r11, #36, 0 ; 0x24
0x000080cc <+40>:   bl  0x806c <sum>
0x000080d0 <+44>:   sub sp, r11, #12, 0
0x000080d4 <+48>:   ldm sp, {r11, sp, lr}
0x000080d8 <+52>:   bx  lr
```

Look at the assembly for `make_array` and sketch a diagram of its stack frame.

- After the instructions for the standard prolog, which instruction makes space for the array local variable?
- How are the contents for the array initialized (or not)?
- If the code erroneously wrote to an index past the end of a stack-allocated array, what data would be corrupted?  What kind of consequence might result from this error?
- If the code erroneously returned the address of a stack-allocated array, what might happen when the caller try to access the memory at that address?


#### Solving a stack mystery
Your newfound understanding of how the stack is managed is a neat superpower that you can now test out. Here is the [mystery diagram](images/stack_mystery.html) of the `mystery.c` program stopped somewhere else in its execution. In this diagram, the stack is not annotated with labels and color-coded frames as before. The stack memory looks to be a inscrutable jumble of hex numbers.  However, the current value of `fp` and `sp` are marked. How can you use these anchors to get a foothold to the stack frame of the currently executing function? How can you then work backwards from there to the frame of the caller and its caller and so on? Manually unwind and annotate the stack memory with labels to identify each stack frame and its contents. You have just produced your first backtrace!

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
$ arm-none-eabi-nm -n start.o cstart.o
```

What symbols are listed for `start.o`? For `cstart.o`? How do the symbols listed correspond to the functions defined in the source files?
What is the significance of the number shown in the left column for each symbol?
What do each of the single letters `T`, `U`, and `t` in the second column mean? 

Skim the man page to learn a little bit about this tool and the variety of symbol types:
```console
$ man arm-none-eabi-nm
```
Our modules will typically contain text (code) symbols and data symbols (with variants common, uninitialized, read-only). What is the significance of upper versus lowercase for the symbol type? What does the `-n` flag do? What about the `-l` flag? Try adding it to your call to `arm-none-eabi-nm` and see what happens.

Let's look at the symbols in a more complex object file. Review the variable definitions in the source file `linking.c`. Build `linking.o` and view its symbol list:
```console
$ make linking.o
$ arm-none-eabi-nm -n linking.o
```
How many symbols are listed for `linking.o`?  What do the single letter symbols `D`, `R`, `C`, and `b` mean in the `nm` output?

Can you match each function/variable definition in `linking.c` to its symbol in the `nm` output? A few of the variables defined seem to have been completely optimized out, what made that possible? None of the parameters or stack-local variables in `linking.c` are listed as symbols, why not?

What type and size of symbol would correspond to an array definition such as  `const int[5]`? See for yourself by uncommenting the declaration on line 13 of `linking.c`, rebuild and view `arm-none-eabi-nm -S linking.o`.

#### Symbols in an executable

After compiling each individual source file into an object file, the final build step is to link the object files and libraries into a program executable. The three object files we examined above are linked together in `linking.elf`.  Use `make linking.elf` to perform the link step and then use `nm` to look at the symbols in the final executable.

```console
$ make linking.elf
$ arm-none-eabi-nm -n linking.elf
```
The executable contains the union of the symbols in the three object files. What is the order of the symbols in the executable? How have the symbol addresses changed during the link process?
Do any undefined symbols remain?
What happened to the symbols previously marked `C`?

#### Resolution and relocation

The Makefile in this project has additional action that creates a `.list` file for each build step. These listing files contain the disassembly of the compiled module.  Let's look into those listings to get a better understanding of how symbols are resolved and relocated by the linker.

```console?prompt=$
$ cat start.o.list

00000000 <_start>:
       0:   mov     sp, #134217728  ; 0x8000000
       4:   mov     fp, #0
       8:   bl      0 <_cstart>

0000000c <hang>:
       c:   b       c <hang>
```

The third instruction is where `_start` calls `_cstart`. This *branch and link* instruction `bl` has 0 at the target destination address.
This target is labeled `<_cstart>`, but 0 doesn't seem quite right. In this module, 0 is the address of `_start`.  Hmm, most curious...

The listing for `linking.elf` begins with the instructions for `_start` but this is _after_ linking. What do you notice that is different now?

```console?prompt=$
$ cat linking.elf.list
 00008000 <_start>:
     8000:       mov     sp, #134217728  ; 0x8000000
     8004:       mov     fp, #0
     8008:       bl      80b8 <_cstart>

 0000800c <hang>:
     800c:       b       800c <hang>
 ...

 000080b8 <_cstart>:
     80b8:       mov     ip, sp
 ...
```

First note that after linking, the addresses (in leftmost column) start at `0x8000` and increase from there. These addresses indicate the location of each instruction in the final executable. Can you work out how each symbol's final address relate to its original offset in the object file?  The process of gathering all symbols from the modules and laying out into one combined package at their final locations is called _relocation_. The linker uses the [memmap](#memmap) to determine how and where to layout the symbols.

In the listing `start.o.list`, the destination address for the branch to `_cstart` was 0.  In the listing `linking.elf.list`, the destination address has been changed to `0x80b8`.  Read further down in the listing to see what is at address `0x80b8`. Makes sense?

In the listing for `linking.o.list` (pre-link), find the instructions for the function `main`. It contains three `bl` instructions, one is a function call to a function defined in this module (`sum`), the other two call functions outside this module (`uart_init` and `printf`).  The call within module has the function offset as destination address, but the calls to outside the module have destination address 0, used as a placeholder. In the listing `linking.elf.list` (post-link), find those same instructions for `main` and you will see all destination addresses are now filled in with the final location of the symbol.

The compiler processes only a single module (file) at a time and thus it can only resolve references to symbols that appear within the module currently being compiled. The linker runs in a subsequent pass to perform tasks that require joining across modules. The process of filling in the missing placeholder addresses with the final symbol locations is known as _resolution_.

The linker is given a list of object files to process and it will combine the files together and arrange symbols into their final locations (relocation) and resolve cross-module references (resolution).

Answer the check-in question on linking.[^3]

#### Memmap

As part of the relocation process, the linker places all of the symbols into their final location. You supply a _memory map_ to the linker to indicate the layout of the sections. Let's look into this file to better understand its purpose and function.

Change to the `lab4/linking` directory and use `nm` to see the final locations of all the symbols in the executable.

```console
$ arm-none-eabi-nm -n linking.elf
```

Note how all symbols of a given type (text, data, rodata, etc.) are grouped together into one section.

Now open the file `memmap` in your text editor. `memmap` is a _linker script_, which tells the linker how to lay out the sections in the final executable file.

- Do you see how the `memmap` linker script specifies how sections from
  individual files are to be combined?

- One of the purposes of `memmap` is to ensure that the symbol `_start`
  appears first in the executable file. Why is it critical that this 
  function be first? How does `memmap` specify where `_start`
  should be placed?

- Another purpose of `memmap` is to setup block storage of uninitialized
  variables. Look for the symbols `__bss_start__` and
  `__bss_end__`. Where are those symbols placed? In a C program, uninitialized global
  variables are to be set to 0 at program start. How does `_cstart` use the bss start/end symbols to
  zero the unset variables?

Our projects all use this same `memmap`, which defines a correct layout for a standard bare-metal C program for the Pi. You are unlikely to need to edit or customize it.  However, it is still worth taking a look at some basic [documentation on linker scripts](https://sourceware.org/binutils/docs-2.21/ld/Scripts.html).

## Check in with TA
<a name="checkin"></a>
Each question below is a diagnostic question for a lab section. Test your understanding by answering the corresponding question as you finish an exercise. We encourage you to check in with a TA after
completing a section or two, while the material is still fresh in your mind.

[^1]: Show us your completed memory diagram for the example program (or even better, the annotated mystery diagram if you deciphered it). Here is our copy of [example](images/stack_example_completed.html) and [mystery](images/stack_mystery_deciphered.html) if you'd like to compare your answers.
[^2]: How should the number of `free` calls relate to the number of `malloc` calls in a correctly-written program that uses heap allocation?
[^3]: Compare the symbol list from `nm linking.o` to `nm linking.elf`. How do the symbol addresses from an object file differ from the addresses in the linked executable?  How does the instruction `bl _cstart` change after linking?
