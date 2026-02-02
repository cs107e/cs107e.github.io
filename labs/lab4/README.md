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
- Experiment with the linker `ld`and  `binutils` tools, with the goal of understanding the structure and contents of
  object and executable files (ELF).
- Diagnose and resolve common build failures.

## Prelab preparation
To prepare for lab, do the following: 

1. Be up to date on [recent lecture content](/schedule/#Week4): __Stack/heap and linking__
    - Give special attention to [recap of stack frames](/lectures/Memory/stack_notes) in readings for Monday's lecture.
    - Read up on [binutils in our guide](/guides/binutils).
1. Organize your supplies to bring to lab
    - Bring your laptop (with full battery charge) and entire parts kit.


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

Change to the directory `lab4/stack`. Review the sample program in the `example.c` file. Have a printed copy of the program's [stack memory at start](images/stopped-main-empty.html) on hand for jotting down notes.

Using `make debug` to build the program and run under `gdb`. Use  `start` to begin executing and stop at `main`.  Below are sample gdb commands you can use to make some observations of the current state when stopped.

```console?prompt=(gdb)
(gdb) start
Temporary breakpoint 2 at 0x400001b8: file example.c, line 32.
Starting program: example.elf

Temporary breakpoint 2, main () at example.c:32
32   int main(void) {
(gdb) list main
(gdb) info address main
(gdb) print $pc
(gdb) disassemble
(gdb) disassemble 0x40000000,0x40000036
(gdb) # Examine memory: x/FMT ADDRESS. ADDRESS 0x40000000, FMT 2 words in hex
(gdb) x/2wx 0x40000000
(gdb) x/2i  0x40000000
(gdb) # Examine memory at current stack top, FMT 4 words in hex
(gdb) x/4wx $sp
```

The [full memory diagram](images/stopped-main-full.html) shows the state of memory up to the point where `main` was called. The information shown in gdb should match this diagram. Here are some key details to observe:
- `info address main` reports that the instructions for function `main` start at address `0x400001b8`.
- `print $pc` to see value of `pc` register, the address of the next instruction to execute. The program is currently stopped at the entry to `main`, thus `$pc` should be the address of the first instruction of main, which is same as reported by `info address main`.
- `disassemble` lists the instructions in the currently executing function (i.e. function containing `$pc`). In this context, it lists instructions of `main`.  The instruction that is next to be executed (i.e. value of `$pc`) is marked with an arrow `=>`.
- Look at the memory diagram and find addresses `0x40000000` to `0x400001e8`. This is the "text" section, containing binary-encoded instructions.  You can specified arguments to `disassemble` command to select a named function or range of addresses. Try it now, and see how instructions match what is shown in the memory diagram.
- The gdb `x` command ("examine memory") is used to display the contents of memory as though it were an array. You specify the number of elements, the size of each element, and display format.
    - Use `help x` to learn more about the options for repeat count, element size, and display format. As an example, `/2wx` displays __2__ __word__-size elements in __hex__ format. ("word" is 32-bits)
        - BTW: the same format characters also apply to the `print` command you learned previously and the `display` command you will learn below, e.g `p/x` to print in hex or `p/t` to print in binary.
    - Try `x/8bx 0x40000000`. This command prints 8 bytes in hex starting at address `0x40000000`.
    - The command `x/2wi 0x40000000` examines the same 8 bytes of memory, this time displaying as 2 word-size instructions.
- The stack pointer was initialized to address `0x50000000` and grows downward when a new stack frame is pushed.  At the point the program is currently stopped, the stack contains only one stack frame, for the function `_cstart`. The frame size is 16 bytes and occupies locations `0x4ffffff0`- `0x4ffffffc` on the stack.
    - Try examine `x/4wx $sp` to display 4 words in hex starting from address `sp`. This command shows the topmost four words on the stack -- neat!
    - This should match what is shown in the memory diagram for the stack frame.

#### gdb commands to single step by assembly instruction
You have previously used `step` to execute a single line of C, today you'll use `stepi` to execute a single assembly instruction. This is Real Deal of single-stepping.

The gdb `display` command adds an expression to be auto-printed after each step. The useful display command `display/i $pc` auto-prints the `$pc`. As you single-step, this auto-print expression will display the instruction that is coming up next.

```console?prompt=(gdb)
(gdb) display/i $pc
=> 0x400001b8 <main>:   add sp,sp,-16
```
Try out using `stepi` to execute the first few assembly instructions one by one.

```console?prompt=(gdb)
(gdb) disassemble main
Dump of assembler code for function main:
=> 0x00000000400001b8 <+0>:     add sp,sp,-16
   0x00000000400001bc <+4>:     sd  ra,8(sp)
   0x00000000400001c0 <+8>:     sd  s0,0(sp)
   0x00000000400001c4 <+12>:    add s0,sp,16
   0x00000000400001c8 <+16>:    li  a1,7
   0x00000000400001cc <+20>:    li  a0,5
   0x00000000400001d0 <+24>:    jal 0x40000100 <combine>
   0x00000000400001d4 <+28>:    jal 0x4000016c <follow_me>
   0x00000000400001d8 <+32>:    ld  ra,8(sp)
   0x00000000400001dc <+36>:    ld  s0,0(sp)
   0x00000000400001e0 <+40>:    add sp,sp,16
   0x00000000400001e4 <+44>:    ret
End of assembler dump.
(gdb)
(gdb) stepi
0x00000000400001bc  32  int main(void) {
1: x/i $pc
=> 0x400001bc <main+4>: sd  ra,8(sp)
(gdb) stepi
0x00000000400001c0  32  int main(void) {
1: x/i $pc
=> 0x400001c0 <main+8>: sd  s0,0(sp)
```

#### Stack operations
- The first few instructions of a function set up the stack frame. These instructions are called the function _prolog_.
- A reciprocal set of instructions at the end of the function deconstruct the frame. These instructions are called the function _epilog_.
    -  The entry to each function will use the same basic prolog sequence (and exit uses same epilog), with minor differences in which registers are saved and the amount of space set aside for local variables.
- The instructions in between the prolog and the epilog are the function _body_.
    - Instructions in the body are specific to the particular function.

> __Pro-tip: don't agonize about function call overhead__ For a simple function, the number of instructions in the prolog and epilog can outnumber the entire body. This sometimes causes folks to become concerned about the performance hit of function calls.  Don't sweat it!  With our supersonic 1Ghz RISC-V processor, these instructions take mere nanoseconds. The benefits of unifying code and decomposition are well worth the time!
{: .callout-info }

#### Trace function prolog
Use `start` to restart the program from the beginning and stop again at entry to `main`.

```console?prompt=(gdb)
(gdb) start
Temporary breakpoint 3 at 0x400001b8: file example.c, line 32.
Starting program: /Users/julie/github/mango-staff/labs/lab4/code/stack/example.elf

Temporary breakpoint 3, main () at example.c:32
32  int main(void) {
(gdb) x/4wi main
=> 0x400001b8 <main>:   add sp,sp,-16
   0x400001bc <main+4>: sd  ra,8(sp)
   0x400001c0 <main+8>: sd  s0,0(sp)
   0x400001c4 <main+12>:    add s0,sp,16
```


Use `x` to see the first four instructions in the function prolog. Carefully read over those instructions, then trace their action in gdb using `stepi` to single-step.  Have your printed memory diagram on hand and as you `stepi`, use `p` and `x` to get updated values of registers and memory and manually update the diagram to add the contents of the stack frame for `main`.  Ask questions and discuss with your labmates to help one another get a complete and accurate understanding of the process.

Here is a walkthrough of the prolog instructions:

1.  `add sp,sp,-16`
    - This first instruction adjusts the stack pointer down by 16 bytes. This sets aside 16 bytes of space on the stack to be used as the stack frame for the function `main`.
    - The RISC-V ABI requires the stack pointer to always be aligned to a multiple of 16, so the adjustment may include padding (e.g. say only 24 bytes were needed, it would adjust the stack pointer by 32 to meet the alignment rule).
2. `sd  ra,8(sp)` and `sd fp/s0,0(sp)`
    - These next two instructions save the values of the `ra` and `fp` registers to the stack.
    - An `sd` instruction stores a register's value to the destination memory address. The `d` is for "double-word" which is 64-bits.
    - Register `x8` has two nicknames: `s0` and `fp`.  These are refer to same register. The gdb disassembly uses `s0` ; I wish it would instead use `fp` to remind of the register's purpose, sigh.  In the lab writeup, we refer to the register as `fp/s0` to keep two names connected in your mind.
    -  The first 16 bytes of a RISC-V stack frame are used to save registers `ra` and `fp/s0`. The `saved ra` is the first pushed to the stack and underneath it is the `saved fp/s0`.
        - `saved ra`
            - What is the `ra` register used for? What will be the value of the `ra` register when starting the function `main`?
           - The `ra` is an address in the text segment.  In your memory diagram, write in that value and draw an arrow that points to that address. Which function is this address contained in?  What is the meaning of that value relative to the currently executing function?
           - Why might it be useful for the frame to store a saved copy of `ra`? What does it tell you about where the function was called from?
        - `saved fp/s0`
            - The `fp/s0` is an address in the stack. Where in the stack does this address point to?
            - Why might it be useful for the frame to contain a saved copy of `fp/s0`? How can this be used as part of a backtrace operation?
3. `add s0,sp,16`
    - The final instruction of the prolog sets the `fp/s0` register as the "anchor" for the current frame.
    - After executing this instruction, where does `fp/s0` point? Relative to this anchor, at what offset can you find the `saved ra`? the `saved fp/s0`?

The prolog is now finished and execution continues with instructions in the body of the `main` function.  The next few instructions set values for the parameters and make a call to the function `combine`. Keep single-stepping with `stepi` until execution enters the `combine` function. Use `disassemble combine` to view its assembly instruction and note that it starts with the same function prolog instructions as `main` did. Trace/single-step through the prolog here, and extend your memory diagram to add the stack frame for `combine`.

Having traced the frame setup twice, hopefully you are getting familiar with how the stack operates. Each function has same prolog instructions: adjust the stack pointer to make space, store the `saved ra` and `saved fp/s0` to the stack, then anchor the `fp/s0`.

Compare your memory diagram with your tablemates. Ask questions of each other and resolve any discrepancies. We want everyone to have a rock solid undertanding of how a stack frame is laid out and what each of the values mean. Check in with us to confirm your understanding. [^1]

#### Function epilog
The last four instructions of `combine` are the function _epilog_. These instructions tear down the stack frame and restore saved registers before returning to the caller. The purpose of the epilog is to reverse the operations done in the prolog, i.e. put everything back to the state it was on function entry. The instructions restore the registers to their saved values and adjust the stack pointer in preparation for returning control to the caller.

Here is a walkthrough of the epilog instructions of `combine`:
1. `ld ra,8(sp)` and `ld s0,0(sp)`
    - These instructions restore the `ra` and `fp/s0` registers to the values they had on entry to the function. The load instruction reads the saved value from stack memory at the known offset where value was written during prolog.
2. `add sp,sp,16`
    -  This adjusts `sp` to remove the frame from the stack.
3. `ret`
    - Return control to the caller and resume execution at the instruction at `ra` address. (i.e. `$pc = $ra`)


#### Local variables in a stack frame
Let's look at another function `make_array` that declares an array as a local variable. The memory for array is located within the function's stack frame.

```c
int make_array(void) {
    int array[6];
    array[2] = 9;
    array[0] = 13;
    return sum(array, 6);
}
```
```console?prompt=(gdb)
(gdb) disassemble make_array
Dump of assembler code for function make_array:
    0x0000000040000130 <+0>:    add sp,sp,-48
    0x0000000040000134 <+4>:    sd  ra,40(sp)
    0x0000000040000138 <+8>:    sd  s0,32(sp)
    0x000000004000013c <+12>:   add s0,sp,48
    0x0000000040000140 <+16>:   li  a5,9
    0x0000000040000144 <+20>:   sw  a5,-32(s0)
    0x0000000040000148 <+24>:   li  a5,13
    0x000000004000014c <+28>:   sw  a5,-40(s0)
    0x0000000040000150 <+32>:   li  a1,6
    0x0000000040000154 <+36>:   add a0,s0,-40
    0x0000000040000158 <+40>:   jal 0x400000c0 <sum>
    0x000000004000015c <+44>:   ld  ra,40(sp)
    0x0000000040000160 <+48>:   ld  s0,32(sp)
    0x0000000040000164 <+52>:   add sp,sp,48
    0x0000000040000168 <+56>:   ret
End of assembler dump.
```

Review the assembly instructions above for `make_array` and make a sketch of its stack frame.

- How much additional space is allocated on the stack for the array local variable? At what stack offset is the base address of the array?
- How are the contents of the stack array initialized (or not)?
- If the code erroneously wrote to an index past the end of this stack-allocated array, what data would be corrupted?  What kind of consequence might result from this error?
- If the code erroneously returned the address of a stack-allocated array, what might happen when the caller try to access the memory at that address?

Writing outside the bounds of a stack-allocated array can be a devastating error.  The array is located in stack memory near critical values such as the `saved fp/s0` and `saved ra`.  Writing past the array can overwrite these precious saved values which then causes execution to "teleport" to a completely nonsensical place. Even writing a single extra byte is too far if the array immediately abuts a critical saved value. Ouch! The effect of stack corruption can be a baffling phenonmenon the first time you encounter it. Knowing how the stack housekeeping is managed provides helpful insights for recognizing the situation and debugging it.

#### Solving a stack mystery

Your newfound understanding of how the stack is managed is a neat superpower that you can now test out. Here is a [mystery diagram](images/mystery.html) of the example program stopped somewhere else in its execution. In this diagram, the stack is not annotated with labels and frame divisions as before. The stack memory looks to be a inscrutable jumble of hex numbers.  However, the current value of `fp/s0` and `sp` are marked. How can you use these anchors to get a foothold to the stack frame of the currently executing function? How can you then work backwards from there to the frame of the caller and its caller and so on? Manually unwind and annotate the stack memory with labels to identify each stack frame and its contents. You have just produced your first backtrace!

Compare your completed memory diagram with your neighbors and confirm that your versions agree. Here is our version of the [mystery deciphered](images/mystery-deciphered.html).

### 2. Heap

Change to the directory `lab4/heapclient` to begin your exploration into heap allocation.
So far we have stored our data either as local variables on the stack or global variables in the data segment. The functions `malloc` and `free`  offer another option, this one with precise control over the size and lifetime and greater versatility at runtime.

Study the program `heapclient.c`. The `tokenize` function is used to 
dissect a string into a sequence of space-separated tokens. It calls the (not-yet-implemented) function `char *strndup(const char *src, size_t n)` to make a copy of each token. Your job is to implement the `strndup` function to work as intended, i.e., return a new string containing the first `n` characters of the `src` string.

Talk over with your neighbor why it would not be correct for `strndup` to declare a local array variable in its stack frame to store the new string.  When a function exits, its stack frame is deallocated and the memory is recycled for use by the next function call.  What would be the consequence if `strndup` mistakenly returns a pointer to memory contained within its to-be-deallocated stack frame?

Instead `strndup` must allocate space from the heap, so the memory will persist after the function exits. The `strndup` should call `malloc` to request the necessary number of bytes. How many total bytes of space are needed to store a string with `n` characters?

Now that you have the necessary memory set aside, what function from the `strings` module can you call to copy the first `n` characters from the `src` string to the new memory? What is the final step that completes the new string?

Once you have completed your implementation of `strndup` to make a proper heap copy of the string, build and run the program to verify your code is correct.

Unlike stack and global memory, which is automatically deallocated on your behalf, you must explicitly free dynamic memory when you are done with it. For the finishing touch, edit `main` to add the necessary calls to `free` to properly deallocate all of the heap memory it used. You're ready for this check-in question.[^2]

### 3. Linking and memmap

In this exercise, you will repeat some of the live coding demonstrations from the lecture on linking and loading.

Let's review some terminology. An _object file_ (also called an .o file or a relocatable) is the result of compiling and assembling a single source file. An object file is on its way to becoming a runnable program, but it's not complete until is has been linked.  The linker joins one or more object files and any libraries, resolves inter-module references, and relocates symbols to their final location. The output of the linker is an _executable_ file, this represents a full program that is ready to run.

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
Our modules will typically contain text (code) symbols and data symbols (with variants common, uninitialized, read-only). What is the significance of upper versus lowercase for the symbol type?

Let's look at the symbols in a more complex object file. Review the variable definitions in the source file `program.c`. Build `program.o` and view its symbol list:
```console
$ make program.o
$ riscv64-unknown-elf-nm -n program.o
```
How many symbols are listed for `program.o`?  What do the single letter symbols `D`, `R`, and `b` mean in the `nm` output?

Can you match each function/variable definition in `program.c` to its symbol in the `nm` output? A few of the variables defined seem to have been completely optimized out, what made that possible? None of the parameters or stack-local variables in `program.c` are listed as symbols, why not?

#### Symbols in an executable

After compiling each individual source file into an object file, the final build step is to link the object files and libraries into a program executable. The three object files we examined above are linked together in `program.elf`.  Use `make program.elf` to perform the link step and then use `nm` to look at the symbols in the final executable.

```console
$ make program.elf
$ riscv64-unknown-elf-nm -n program.elf
```
The executable contains all of the symbols in the combined three object files. What is the order of the symbols in the executable? How have the symbol addresses changed during the link process? Do any undefined symbols remain?

Remember that they compiler processes only a single module (file) at a time and thus it can only resolve references to symbols that appear within the module currently being compiled. The linker runs in a subsequent pass to perform tasks that require joining across modules. The process of filling in the missing placeholder addresses with the final symbol locations is known as _resolution_.

The linker is given a list of object files to process and it will combine the files together and arrange symbols into their final locations (relocation) and resolve cross-module references (resolution).

Answer the check-in question on linking.[^3]

#### Linker script `memmap.ld`

As part of the relocation process, the linker places all of the symbols into their final location. You supply a _memory map_ to the linker to indicate the layout of the sections. Let's look into this file to better understand its purpose and function.

Change to the `lab4/linking` directory and use `nm` to see the final locations of all the symbols in the executable.

```console
$ riscv64-unknown-elf-nm -n program.elf
```

All symbols of a given type (text, data, rodata, etc.) are grouped together.

Open the file `memmap.ld` in your text editor. `memmap.ld` is a _linker script_, which tells the linker how to lay out the sections in the final executable file.

- The names for the standard sections are a bit cryptic. What information goes in the `text` section? The `rodata` section? The `data` section? The `bss` section?

- One of the jobs of `memmap.ld` is to ensure that the symbol `_start`
  appears first in the executable. Why is it critical that this
  function be first? How does `memmap.ld` specify where `_start`
  should be placed?

-  `memmap.ld` also is responsible for configuring the block storage used for uninitialized
  variables. It establishes the block boundaries by placing the markers `__bss_start` and
  `__bss_end` and [memmap.h](/header#memmap) defines constants for those locations. The C standard requires that uninitialized global
  variables have the value zero at program start. The `_cstart` function (in `cstart.c`) zeroes out the memory for entire block as part of the start up sequence before entering the `main()` function.

All of the assignments, labs, and demo projects use our custom linker script `$CS107E/lib/memmap.ld` ([browse memmap.ld](/src#memmap)) for a bare-metal C program on Mango Pi. You are unlikely to need to edit or customize it.  However, if you curious to learn more, see the [documentation on linker scripts](https://sourceware.org/binutils/docs-2.21/ld/Scripts.html).

### 4. Diagnosing build errors

A benefit to learning the tools and steps of the build process is that this understanding gives you the needed insight to diagnose and resolve build errors.

Change to the `build_errors` directory of `lab4`. There are several subfolders, each containing a small project that has a build error.

Change to each directory and try to build using `make`. The build process will fail. Your goal is to use the information reporting by the build toolds to diagnose what's wrong and fix the project so it builds successfully.
- Which tool is reporting a problem? (preprocessor, compiler, assembler, linker, make, ...)
- What is the warning/error message? What is this message trying to communicate?
- What changes are needed to correct the problem?

You're ready for the final check-in question.[^4]

## Check in with TA
The key goals for this lab are to leave with a good understanding of how the stack operates and to have practiced with build tools and diagnosing build errors. These are skills that will serve you well in future! [^5]

<div class=checkinsheet markdown="1">
# {{ page.title }}
<div class="underline-name"></div>
Circle lab attended:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  _Tuesday_  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  _Wednesday_
<BR>
<BR>
Fill out this check-in sheet as you go and use it to jot down any questions/issues that come up.  Please check in with us along the way, we are here to help![^6]
</div>

[^1]: Show us your completed memory diagram for the example program stopped in `combine` (or even better, the annotated mystery diagram if you deciphered it). Here are our annotations for [stopped in combine](images/stopped-combine.html) and [mystery deciphered](images/mystery-deciphered.html) to compare with yours.
[^2]: How should the number of `free` calls relate to the number of `malloc` calls if correctly using heap allocation?
[^3]: How do symbol addresses in a `.o` file (pre-link) differ from `.elf` file (post-link)? Use `nm` to compare. How does the instruction `jal _cstart` change from `.o` to `.elf`? Use `objdump -d` to view disassembly.
[^4]: Explain the difference in a build error that results from forgetting to `#include "module.h"` versus forgetting to link against `module.o`.
[^5]: Are there any tasks you still need to complete? Do you need assistance finishing? How can we help?
[^6]: Do you have any feedback on this lab? Please share!
