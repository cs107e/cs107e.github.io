---
released: true
permalink: /assignments/assign4/
title: 'Assignment 4: Backtrace and Malloc'
toc: true
---
{% comment %}
Task list to copy/paste when creating PR for this assign:

__Before releasing assign4:__
- [ ] Review writeup/starter code (instructor)
- [ ] Review consistency/completeness of grading info published to students relative to grading tests used, consider changes clarity/ease of grading (TA)
- [ ] Followup on issues from previous quarter postmortem (issue #337)

__To prep for assign4:__
- [ ] 

{% endcomment %}

*Written by Julie Zelenski*

{% include duedate.html n=4 %}

## Goals
For this week's assignment, you continue in the role of library implementer to add a debugging aid (`backtrace.c`) and a heap allocator (`malloc.c`) to your growing collection of foundational routines. 

Global and local variables have been sufficient for our needs thus far, but 
to support more complex applications we'll need support for dynamic allocation.
Heap allocation offers the programmer explicit and  direct control over sizing and lifetime of storage. With this power comes the responsibility to properly allocate and deallocate that memory. Using dynamic memory correctly and safely is a challenge for the client; implementing a heap allocator correctly and efficiently is a challenge for the implementer.

After you finish this assignment, you will:

- be able to dissect the runtime stack in an executing program,
- further understand C data structures and memory layout,
- have more experience managing pointers in C, and
- appreciate some of the complexity and tradeoffs in implementing a heap allocator.

Each new assignment also provides many opportunities to appreciate and value your hard work in previous weeks. You'll come to love how much easier _your_ `printf` makes your testing and debugging.  By the end of the quarter, almost every single  instruction in your fully functioning console will be from code you have written and know well: you'll understand your whole system from the ground up.

## Get starter files
Do a `git pull` in your $CS107E repository to ensure the courseware files are up to date.

```console
$ cd $CS107E
$ git pull
```

Now cd to your local `mycode` repo and pull in the assignment starter code:

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull --allow-unrelated-histories starter-code assign4-starter
```

In the `assign4` directory, you will find these files:
- `backtrace.c`, `malloc.c`:  library modules 
- `test_backtrace_malloc.c`:  test program with your unit tests for backtrace and malloc
- `heap_workflow.c`:  simulation application that runs a generated workflow against your heap allocator and reports on correctness and performance
- `Makefile`: rules to build heap_workflow application (`make run`) and unit test program (`make test`)
- `README.md`: edit this text file to communicate with us about your submission

The `make run` target builds and runs the sample application `heap_workflow.bin`. This target is most useful late in the cycle as a final confirmation of your completed allocator. The `make test` target builds and run the test program `test_backtrace_malloc.bin`. This test program is where you will add all of your unit tests. You will make heavy use of this target throughout your development. You can run the debugger in simulator mode on the corresponding ELF file, e.g. `arm-none-eabi-gdb test_backtrace_malloc.elf`.

You can edit `MY_MODULE_SOURCES` in the `Makefile` to choose which modules of yours to build on. (See instructions for [use of MY_MODULE_SOURCES](/assignments/assign3/#mymodules) in assignment 3.)


## Core functionality

### Backtrace module
The `backtrace` module provides functionality to gather and print a stack backtrace. A backtrace is a listing of the function call stack beginning at the currently executing  function, showing the sequence of calls that led to this function, and ending at `_cstart`.

Consider a program whose `main` function calls `diff`, which then calls `abs`. A backtrace initiated during `abs` could look something like this:

```console?prompt=(gdb)
#0 0x8020 at abs+16
#1 0x80d8 at diff+20
#2 0x80fc at main+24
#3 0x814c at _cstart+40
```

Start by reviewing the header file for the module (available as `$CS107E/include/backtrace.h` or [browse backtrace.h here](/header#backtrace)). The functions exported by the `backtrace` module are:

- `int backtrace(frame_t f[], int max_frames)`
- `void print_frames(frame_t f[], int n)`
- `void print_backtrace(void)`
- `const char *name_of(uintptr_t fn_start_addr)`

The two `print_xxx` functions are given to you pre-written in `backtrace.c`.  You are to implement the `name_of` and `backtrace` functions. `name_of` finds the name for a given function and  `backtrace` fills an array of stack frame information. These two functions are small but mighty. Completing them will demonstrate your  understanding of the stack layout and show off your finest pointer skills.

Before writing any code for the backtrace module, we recommend that you first spend time poking around in `gdb` and exploring the runtime state of the program, examining stack  memory, stepping through function calls, and manually accessing the stack frame data, and so on. The stack mystery task in [Exercise 1 in Lab 4](/labs/lab4/#solving-a-stack-mystery) could be worth a second look.

#### Harvesting a stack frame
The `backtrace` function fills an array of structs, one `frame_t` for each function  on the stack. See `backtrace.h` for the definition of the type struct `frame_t`, including the comments which explain the intent of the fields `resume_addr` and `resume_offset`.

The `backtrace` function starts at the innermost stack frame and traces to the outermost, harvesting the information for each stack frame and storing into a `frame_t`.

The current value of the `fp` register is the "anchor" you need to start examining the stack. You must drop down to assembly to access a register. The GCC `__asm__` keyword allows you to mix assembly in with your C code. The code sample below demonstrates using embedded assembly to store the value of the `fp` register into a variable 
named `cur_fp`:

```c
uintptr_t *cur_fp;
__asm__("mov %0, fp" : "=r" (cur_fp));
```

> The syntax for inline assembly in GCC is a bit quirky; don't feel you need to make a deep dive into it. You can simply use the above snippet as-is to grab the value of the fp register into the C variable `cur_fp`. If you're
curious to learn more, check out the manual:
[gcc support for inline assembly](https://gcc.gnu.org/onlinedocs/gcc/Using-Assembly-Language-with-C.html).
{: .callout-info}

For the lab 4 example program, you manually filled in stack frames and added to your memory diagram. Have that [completed diagram](/labs/lab4/images/stack_example_completed.html) handy when working through the following thought questions.

- The frame for the currently executing function is topmost on the stack.  The current `fp` points to the first value of this topmost stack frame. Which value can you look at within the stack frame in order to tell which function is currently executing? Which value in the stack frame contains the resume address?
- Which value in the stack frame contains the saved `fp`?  How can the saved `fp` be used to access to the stack frame for the caller?  How do you then go from that frame to the frame of its caller and so on? What is the indication that you have hit the outermost stack frame? This is where backtrace stops.
- The `resume_offset` is calculated by subtracting the address of the first instruction of the caller from the resume address in the caller. What information in the stack frame for `combine` helps you locate its first instruction?
- Confirm your answers to the above questions by running your test program under the debugger, set a breakpoint at the point you want to backtrace, and then when stopped at the breakpoint, use gdb commands to print registers and examine memory:
```console?prompt=(gdb)
(gdb) print $fp
$1 = (void *) 0x7fffc6c
(gdb) p $sp
$2 = (void *) 0x7fffc58
(gdb) p/x $lr
$3 = 0x851c
(gdb) p *(void **)$fp
$4 = (void *) 0x802c <binky+12>
(gdb) x/10wx $sp
0x7fffc58:  0x00000001  0x00000000  0x07ffffdc  0x07fffc70
0x7fffc68:  0x0000851c  0x0000802c  0x00000000  0x00000000
0x7fffc78:  0x00000000  0x00000000
```
- One piece of advice that cannot overemphasized is that __pointer arithmetic scales by the size of the pointee type__. The expression `ptr + 2` when `ptr` is type `char *` is simply adding 2 to the base address `ptr`. However, if `ptr` is type `int *`, the expression  `ptr + 2` is adding 8 (2*sizeof(int)) to `ptr`. This will come up when using offset arithmetic to access relative locations within the stack. A location that is an offset of 12 bytes from a `char *` base can alternatively be computed as an offset of 3 words from a `int *` base, so be sure to consider pointee type when determining appropriate value for offset.

#### Obtaining a function's name
The final task of backtrack is to associate the name of the function with each stack frame. Symbol names are usually not present in a binary file as functions are referred to by address, not name. However, there is a compiler option to retain function names in the binary to support developers (such as yourself) writing debugging tools. The starter Makefile has `-mpoke-function-name` enabled in `CFLAGS`. This option tells gcc to store a function's name with the compiled instructions, embedded in the memory preceding the function's first instruction.

> The documentation below for the `mpoke-function-name` option is taken from the gcc manual, but we have corrected a few mis-identified details. Please use this version rather than refer back to the original.
{: .callout-danger}

<style>
.quote {
    font-size: small;
    padding:  0 30px;
}
</style>
<div class=quote markdown="1">
`-mpoke-function-name`

Write the name of each function into the text section, directly
preceding the function prologue. The generated code is similar to
this:

```
t0
    .ascii "arm_poke_function_name", 0
    .align
t1
    .word 0xff000000 + (t1 - t0)
arm_poke_function_name
    mov     r12, sp
    push    {fp, r12, lr, pc}
    sub     fp, r12, #4
```

When performing a stack backtrace, code can inspect the value of the word at the
location immediately preceding the first instruction. If that value 
has the most significant 8 bits set, then we know that there is a
function name embedded that precedes value and the name has
length `(value & ~0xff000000)`.
</div>

The `name_of` function is given the address of a function's first instruction and finds the embedded function name, if available. If the word preceding a function's first instruction in memory has `0xff` in its most significant byte, this indicates that the function has its name embedded. The size of the function name is stored in the lower three bytes of the word. Use that size to back up in memory to the start of the string. Neat! If you don't find the magic `0xff` in the right place, this indicates the function name is not available, in which case, `name_of` should return `"???"`.

The embedded function name is null-terminated and the terminator is counted in the size. The `.align` directive rounds up the size to a multiple of 4; additional null characters are used as padding.  A function named `binky` will have its name embedded as `binky\0\0\0` and size of 8. Given that the name string is stored with its null terminator, `name_of` can access the string in-place and simply refer to it by address without making a copy of of the string.

If the example program from lab4 is compiled `-mpoke-function-name`, the text section will now contain the function names embedded alongside the instructions. Here is the [memory diagram with embedded function names](images/diagram_with_poke.html). Using this diagram as a guide, work out by what steps you could access a function's name if starting from the address for the function's first instruction. This is  what needs to happen in `name_of`.

Put this all together and you have a module capable of harvesting a backtrace for the current stack, including function names. It requires surprisingly little code, but it's dense and heavy on the pointer arithmetic and typecasts. Carefully map out your approach, take it slow, and use `gdb` as you go to examine memory and print expressions to confirm your steps. Validating your backtrace against `gdb`'s `backtrace` command can also be a useful debugging technique.

All stack frames use the same layout, so once your code can correctly backtrace one stack, that code is quite likely to correctly handle all other stacks without additional special cases. Confirm with a few tests and you're done with this module!

### Malloc module

The job of the heap allocator is to service allocation requests and manage the pool of available space.

Start by reviewing the header file (available as `$CS107E/include/malloc.h` or [browse malloc.h here](/header#malloc)). The functions exported by the `malloc` module are:

- `void *malloc(size_t nbytes)`
- `void free(void *ptr)`
- `void *sbrk(int nbytes)`

These functions operate in the same spirit as the same-named allocation 
functions of the standard C library. 

The essential requirement for any heap allocator is that it correctly service any combination of well-formed requests.  It is also desirable that it be efficient in use of space (recycling freed memory, compactly storing blocks) and efficient in use of cycles (quickly servicing requests). For the assignment, your primary goal will be to achieve correctness, with a very modest expectation on efficiency.

#### Malloc specification

Other than the non-negotiable stipulation to correctly service all valid requests, there are very few requirements to the operational specification of `malloc` and `free`. 

All pointers returned by `malloc` are required be aligned to 8 bytes, the size of the largest primitive type on our system. 

If `malloc` cannot fulfill an allocation request (such as a request that is larger than the available space), it returns `NULL`.  Two valid, but oddball, cases are `malloc(0)` and `free(NULL)`.  The expectation for these edge cases is documented in the `malloc.h` interface.

If a client request is invalid, such as an attempt to `free` a pointer
that was not obtained from `malloc` or `free`ing an already freed pointer, the behavior  of the allocator is undefined. We will not test such invalid calls.

#### Order of attack
Below we offer our suggestions for a step-by-step plan  of the tasks before you. 

1. **Study starter code.**

    The file `malloc.c` in the starter code contains the super simple "bump allocator" shown in lecture. Start by reviewing this code. The bump allocator is correct, but quite naive. It serves as a reference point from which your own heap allocator will be able to show substantial improvements.
   
    Make sure you understand what each line in the bump allocator is doing. Your allocator will be significantly more complicated, so it is best to start from a solid foundation. Here are some self-test questions to work through to verify your understanding:
   
    + Trace the given code for `sbrk`. You will use this code as-is and it is important that you understand exactly what this function does for you and how to use it. Where does the heap segment start in memory? Where does it end?  What is the return value of a call to `sbrk`?  
    + How does `sbrk` enlarge the segment? What is the limit on how much the segment can be enlarged and why is that limit needed? What is the purpose of inline assembly used here?  What happens if you attempt to set the break beyond that limit? 
    + The given code casts the `void *` pointer to `(char *)` before doing pointer arithmetic. What does this typecast do and why is it needed?
    + When servicing a request for `malloc(5)`, how much space is actually reserved by the bump allocator? 
    + Every allocation is required to be 8-byte aligned (i.e. payload must start on an address which is a multiple of 8). How does the bump allocator achieve this requirement?
    + Dissect the crazy-dense `roundup` macro. Plug in a few numbers, (5, 16, 18) and work it out how it operates. Why does the bitwise trick only work for multiples of numbers that are powers of 2?
    + Why is the bump allocator unable to recycle a previous allocation that has been freed?

    The `make run` target runs the simulation program `heap_workflow.c` that processes a generated sequence of random malloc/free requests. The simulation confirms that the allocator correctly services the requests and reports on its performance. Review the code in `heap_workflow.c` to see how the simulator operates. The general gist is that the simulation program tracks the in-use blocks and confirms the blocks are properly managed. It also times the operations (using your handy timer module) and tracks the growth of the bounds of heap. At completion of the simulation, it reports the measured time and space efficiency.

    The `heap_workflow.c` simulation program is ~250 lines, about 20% longer than our reference malloc implementation. Sometimes it takes more code to test a module than it did to write it in the first place! Our simulation program will be useful as a final check on your working allocator.

    Use `make run` right now on the starter code to see the bump allocator in action. The bump allocator is correct but lazy. It process requests very quickly but remorselessly chews through memory with no hope of recycling. You can do better!

2. **Re-work `malloc()` to add a block header to each block.**
    
   Your heap allocator will place a header on each block that tracks the block size and status (in-use or free).  Review these [block header diagrams](block_headers/) to understand the basic layout of a heap using block headers.
   
   The bump allocator services a `malloc` request by extending from the break to place the new block at the previous heap end. Change malloc to also include additional space for a block header and fill that header with the block status and size.
   
   When adding the header, be sure to consider how it will affect payload alignment. Each payload must start on an address that is a multiple of 8. One convenient way to adhere to the alignment rule is to round up each payload to a size that is a multiple of 8, add an 8-byte header, and lay out blocks end to end beginning at an address that is a multiple of 8. Our standard `memmap` aligns the `__bss_end__` symbol to an 8-byte boundary.

3. **Implement `free()` to update status in block header.**
   
   `free()` should access the block's header and mark it as free. Accessing the neighboring memory at a negative offset via pointer subtraction works just like pointer addition -- handy!
   
   This doesn't yet recycle a free block, but marking it as free is the first step toward that.

4. **Implement `heap_dump()` to output debugging information.**
   
   Before things get any more complex, implement a routine to give you insight into what's going on inside your heap.  The intention of `heap_dump()` is to print a visual dump of your heap contents that will help you when debugging your allocator.  Yay `printf`, where have you been all my life?!  
   
   As a starting point, we suggest that `heap_dump` walk the heap and print each block. Starting from the first block, traverse from block to block until you reach the heap end. You can move from a block header to its forward neighbor by using pointer arithmetic to advance by the block size. The block headers are said to form an _implicit list_.
   
   For each block, print the payload address and size and status. You might also elect to print the payload data. If so, we recommend abbreviating by perhaps displaying only the first 16 bytes. If you instead print out every single byte, the output can become overwhelming for larger heaps. 
   
   Use the `trace_heap()` test in `test_backtrace_malloc.c` to try out `heap_dump`. You can add calls to `heap_dump()` wherever desired to get a visual dump of your heap contents for debugging. If you inspect the dump and see that your heap is in a corrupted state, add calls to dump earlier in the sequence and try to pinpoint the exact operation where your heap went from good to bad. Having an inside view of what's going on inside your heap is an invaluable aid for debugging.

   What you print in `heap_dump` will not be graded. This debugging function is yours to use as you see fit, but the more help it provides to you, the better off you will be. 

5. **Upgrade `malloc()` to search freed blocks and reuse/split.**
      
   Now change `malloc` to recycle a free block if possible. Walk the heap by traversing the implicit list of headers, and look for a free block of an appropriate size. A block that exactly matched the requested size would be ideal, but searching the entire heap to find that best fit block can be time-consuming. A quicker approach is to go with _first fit_, i.e. grab the first block found that is large enough. If this block satisfies the request with enough excess leftover to form a non-empty second block, split that remainder into its own free block.
   
   If no recyclable block is found, then just extend the heap and place the new block at the end as before.
   
   Add your own `test_recycle` case to `test_backtrace_malloc.c` to test that your allocator will now recycle freed memory. For example, if you run a loop that repeatedly does malloc-free (same size malloc each time), the same block should be repeated re-used.  Now add a `test_split` case that does a large-size malloc and frees it, followed by several small-size mallocs to see the splitting of the previous large block.  Make liberal use of `heap_dump` to confirm the internal configuration of your allocator during these operations is as you intend. 

6. **Upgrade `free()` to coalesce adjacent free blocks.**
   
   Reusing single freed blocks is a good improvement, but there is further opportunity for recycling. What happens if the client repeatedly allocates 8-byte blocks until the heap is full, frees all of them, then tries to allocate a 32-byte block? The heap contains much more than 32 total bytes of free space, but it has been fragmented  into individual 8-byte blocks.  To service a larger request, we'll need to join those smaller blocks.
   
   Modify `free()` so that when freeing a block, it checks if the block's forward neighbor is also free, and if so, it will _coalesce_ the two blocks into one combined free block. It then considers the forward neighbor of that bigger block to see if it's free and can also be coalesced. Continue forward-coalescing until no further coalescing is possible. Coalescing is only applied in the forward direction as our design does not support efficient access to the backward neighbor.

   Add a `test_coalesce` case to `test_backtrace_malloc.c` that runs a sequence designed for forward coalesce, such as request several small blocks, free the blocks in reverse order, and then request a large block.  Use `heap_dump` to confirm the coalescing and re-use of previous blocks.


7. **Watch your heap at work!**

    After confirmed your allocator's correct behavior on your own test cases, you're ready for a final spin using the simulation application program `heap_workflow.c`. Use the `make run` target to run the simulation. The simulation runs a long sequence of random malloc/free calls, confirming that each request is correctly serviced and payload data is properly managed. It also measures the time spent servicing each request (thank you, timer module!) and observed the growth in the footprint of the heap segment. At the end of the simulation, it reports summary statistics on the allocator performance. Compare your results to the original results of the lazy bump allocator and take a bow for your hard work showing up that slacker!


__Congratulations!__ You have reached the summit of Heap Allocator Mountain ⛰ and earned your Pointer Wrangling Merit Badge 🏆 Time to celebrate with a nice cup of tea ☕️, a moment of mediation 🧘‍♀️, or a nap in the warm sun ☀️.

## Testing <a name="testing"></a>
Your primary testing vehicle is the test program `test_backtrace_malloc.c` that corresponds to the `make test` target. Add your test cases for backtrace and malloc here.  The starter code provides example tests to get you started but you will need to add many more tests of your own to get comprehensive coverage for your modules. A portion of your assignment quality assessment is awarded based on the quality and breadth of the test cases you submit.

The `make run` target runs our `heap_workflow.c` application program to exercise your allocator on a simulated workflow.  This simulation is designed for stress testing and finishing step.   While in the stage of active development, the simulation can be more complicated than helpful; you are likely better off working with small, specific tests that you hand-constructed in the test program. But once your allocator is complete, the simulation is super handy to confirm it is working correctly in large stress test. You can also edit `heap_workflow.c` to tweak the simulation parameters to observe the performance in different scenarios.

## Extension: Mini-Valgrind
The extension for this assignment is to combine your backtrace and malloc modules to add a "Mini-Valgrind" set of memory debugging features to your heap allocator to report on memory errors and memory leaks.

What happens if a client allocates a block of memory and then mistakenly writes
outside of the bounds?  The erroneous write not only stomps on the neighboring payload data, it is almost certain to destroy the neighboring block header. A memory error that corrupts this critical heap housekeeping will lead to many sad consequences. 

What happens if a client allocates a block, but forgets to free it when done? This creates a _memory leak_, where the block is parked there forever, unable to be recycled. A memory leak is much less devastating than a memory error, but can degrade performance over time and eventually cause issues.

Memory debugging tools such as [Valgrind](http://valgrind.org) are invaluable weapons in the battle against difficult-to-find memory errors and memory leaks. For the extension, you are to implement a "Mini-Valgrind" that adds memory error and memory leak detection to your heap allocator.

> __This extension will require a somewhat invasive re-structuring of malloc/free.__ Before attempting the extension, commit, tag, and submit your completed code for the core features. This establishes a known working version that can serve as a point of comparison as you move forward and it may be your lifeline if you end up needing a known good place to restore to.
{: .callout-warning}

#### Red zones and leak detection

One technique for detecting memory over/underruns is to surround each payload with a pair of _red zones_. When servicing a malloc request, oversize the space by an extra 8 bytes. Place the actual payload in the middle with one 4-byte red zone before it and another 4-byte red zone after it. (Take care to keep the payload aligned on the 8-byte boundary). Fill the red zone with a distinctive repeating value (a good use for your handy `memset` function!). Our implementation uses `0x7e`, though you are free to use [any non-offensive pattern you like](https://en.wikipedia.org/wiki/Hexspeak). When the client later frees that block, check the red zones and squawk loudly if the value has been perturbed.

In addition, you should tag each allocated block with the context it was allocated from by recording a "mini-backtrace". Modify your block header structure to add an array of three `frame_t` and fill with a snapshot of the innermost three frames at the point this block was allocated. 

Modify `free` to verify that the red zones are intact for the block currently being freed. If not, print an error message about the block in error along with its mini-backtrace that identifies where the block was allocated from.

```console
=============================================
 **********  Mini-Valgrind Alert  ********** 
=============================================
Attempt to free address 0x0000a978 that has damaged red zone(s): [457e7e7e] [7e7e7e7e]
Block of size 5 bytes, allocated by
#0 0x8d4c at malloc+192
#1 0x8650 at test_heap_redzones+52
#2 0x86bc at main+56
```

The values for the address, damaged red zone, size, etc. will be specific to the block in error, but please make 
the rest of the error report use the same format and wording as our above sample. (Your graders thank you in advance for taking this care!)

Mini-Valgrind also tracks heap statistics, such as the count of calls to `malloc` and `free` and the aggregate total number of bytes requested. Implement the function `memory_report` that is intended to be used at program completion to print a summary of the total heap usage and list all remaining memory leaks. The mini-backtrace stored in the block header is used to identify the context for each leaked block.

```console
=============================================
         Mini-Valgrind Memory Report          
=============================================
final stats: 42 allocs, 40 frees, 5752 total bytes requested

8 bytes are lost, allocated by
#0 0x8d0c at malloc+196
#1 0x8674 at main+28
#2 0x86ec at _cstart+48

100 bytes are lost, allocated by
#0 0x8d0c at malloc+196
#1 0x868c at main+52
#2 0x86ec at _cstart+48

Lost 108 total bytes in 2 blocks. 
```

Please take care to match the wording and format of our sample memory report in order to ease the graders's job.

As a final detail, work out where you can put a call to `memory_report()` so that Mini-Valgrind can provide leak detection to any program finishes, without having to modify each program's `main` function. Hint: think about how the green LED turns on when your test program runs to successful completion. Also note that `rpi-run.py` waits for an `EOT` to detect program completion, so you will need to move the `uart_putchar(EOT)` currently in `main` to follow after your memory report.

To submit the extension for grading, tag your completed code `assign4-extension`.

## Submitting

The deliverables for `assign4-submit` are:

- implementations of the `backtrace.c` and `malloc.c` library modules
- comprehensive tests for both modules in `test_backtrace_malloc.c`
- `README.md` (possibly empty)

Submit your finished code by commit, tag `assign4-submit`, push to remote, and ensure you have an open pull request. The steps to follow are given in the [git workflow guide](/guides/cs107e-git#assignment-submission).

## Grading

To grade this assignment, we will:

- Verify that your submission builds correctly, with no warnings. Clean build always!
- Run automated tests on your `backtrace` and `malloc` modules
- Go over the test cases you added to `test_backtrace_malloc.c` and evaluate for thoughtfulness and completeness in coverage.
- Review your code and provide feedback on your design and style choices.

Our highest priority tests will focus on the core features for this assignment:

- Essential functionality of your library modules
  - backtrace
    - harvest function name
    - array of stack frames with function names and offsets
  - malloc
    - service any sequence of valid requests
    - recycle free blocks
    - split and coalesce free blocks

The additional tests of lower priority will examine less critical features, edge cases, and robustness. Make sure you thoroughly tested for a variety of scenarios!

## Julie's note on heap alligators
At home, my chatter about "heap allocators" was mistaken for "heap alligators" by my kids, who were alarmed that I would ask my students to wrestle with such fearsome beasts. 

A family trip to Gatorland taught us that a __heap of alligators__ can actually
be quite cute and the adorable drawing from Jane Lange (talented CS107e alum, former CS107e TA, and current MIT PhD student) demonstrates that a tamed heap alligator makes a lovely pet. May you, too, tame your heap alligator and triumph over the perils of dynamic allocation.

![](images/heap_of_alligators.jpg){: .zoom}
![](images/heapalligator.png){: .zoom}
