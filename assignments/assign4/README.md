---
released: true
permalink: /assignments/assign4/
title: 'Assignment 4: Backtrace and Malloc'
attribution: Assignment written by Julie Zelenski
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

{% include duedate.html n=4 %}

## Goals
For this week's assignment, you continue in the role of library implementer to add a debugging aid (`backtrace.c`) and a heap allocator (`malloc.c`) to your growing collection of foundational routines. 

Global and local variables have been sufficient for our needs thus far, but more complex applications will require support for dynamic allocation.
Heap allocation offers the programmer explicit and  direct control over sizing and lifetime of storage. With this power comes the responsibility to properly allocate and deallocate that memory. Using dynamic memory correctly and safely is a challenge for the client; implementing a heap allocator correctly and efficiently is a challenge for the implementer.

After you finish this assignment, you will:

- be able to dissect the runtime stack in an executing program,
- further understand C data structures and memory layout,
- gain more experience managing pointers in C, and
- appreciate some of the complexity and tradeoffs in implementing a heap allocator.

Each new assignment also provides opportunity to appreciate and benefit from your hard work in previous weeks. You will delight in the ease of testing and debugging now that you have added `printf` to your bag of tools.  By the end of the quarter, almost every single instruction in your complete console application will be code you wrote yourself and know well: you'll understand your whole system from the ground up.

## Get starter files
Change to your local `mycode` repo and pull in the assignment starter code:

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull code-mirror assign4-starter
```

In the `assign4` directory, you will find these files:
- `backtrace.c`, `malloc.c`:  library modules  (`symtab.c` also, but only used if doing the symbol table extension)
- `test_backtrace_malloc.c`:  test program with your unit tests for backtrace and malloc
- `heap_workflow.c`:  simulation application that runs a generated workflow against your heap allocator and reports on correctness and performance
- `Makefile`: rules to build heap_workflow application (`make run`) and unit test program (`make test`). Also a `make debug` target that runs the unit test program under gdb.
- `README.md`: edit this text file to communicate with us about your submission

The `make run` target builds and runs the sample application `heap_workflow.bin`. This target is useful late in your development cycle as a final confirmation of your completed allocator. The `make test` target builds and run the test program `test_backtrace_malloc.bin`. This test program is where you will add your unit tests. You will make heavy use of this target early and often during development. You can also run the unit test program under the gdb debugger using the `make debug` target.

Reminder: you can edit `MY_MODULE_SOURCES` in `Makefile` to choose which modules of yours to build on. (See instructions for [use of MY_MODULE_SOURCES](/assignments/assign3/#mymodules) in assignment 3.)


## Core functionality

### Backtrace module
The `backtrace` module provides functionality to gather and print a stack backtrace. A backtrace is a listing of the current call stack beginning at the currently executing function, tracing backwards through the sequence of caller functions and ending at the program start.

The header file for the module is available as `$CS107E/include/backtrace.h` or [browse backtrace.h here](/header#backtrace). The public functions exported by the module are:

- `int backtrace_gather_frames(frame_t f[], int max_frames)`
- `void backtrace_print_frames(frame_t f[], int n)`
- `void backtrace_print(void)`

The `backtrace_print` function is given to you pre-written; you will implement the remaining functions. Completing these functions will demonstrate your understanding of the stack layout and show off your excellent pointer skills.

Before writing any code, we recommend that you confirm your understanding of the stack frame layout by poking around in `gdb`, exploring the runtime state of an executing program, stepping through function calls, and examining the stack memory and register values. The stack mystery of [Exercise 1 in Lab 4](/labs/lab4/#1-stack) could be worth a second look.

#### Harvesting a stack frame
The `backtrace_gather_frames` function fills an array of `frame_t`, one for each function  on the stack. The first entry in the array is filled with the frame corresponding for the topmost function, the subsequent entry is the caller of that function and so on working backwards through the call stack. A backtrace stops at the outermost function `_start`.

To begin harvesting stack frames, the current value of the `fp` register will be your "anchor". You must drop down to assembly to access a register.  Use the `backtrace_asm.s` file to implement a helper function to do this.

In exercise 1 of lab 4, you manually added stack frames to your memory diagram. Have that diagram handy when working through the following thought questions.

- The frame for the currently executing function is topmost on the stack.  After executing the function prolog, where on the stack does the `fp` register point?  At what offset from that location can you find the saved `ra`? the saved `fp`?
- What does the value of the saved `ra` tell you about the caller of the current function?
- How can the saved `fp` be used to access to the stack frame for the caller?  How do you then go from that frame to the frame of its caller and so on? What is the indication that you have hit the outermost stack frame? This is where backtrace stops.
- Confirm your answers to the above questions by running your test program under the debugger, set a breakpoint at the point you want to backtrace, and then when stopped at the breakpoint, use gdb commands to print registers and examine memory.  Validating your backtrace against `gdb`'s `backtrace` command is another useful debugging technique.
```console?prompt=(gdb)
Breakpoint 5, check_backtrace (nframes=1) at test_backtrace_malloc.c:20
20      int frames_filled = backtrace_gather_frames(f, nframes);
(gdb) backtrace
#0  check_backtrace (nframes=6) at test_backtrace_malloc.c:20
#1  0x00000000400002fc in function_A (nframes=<optimized out>) at test_backtrace_malloc.c:31
#2  0x0000000040000320 in function_B (nframes=nframes@entry=6) at test_backtrace_malloc.c:35
#3  0x00000000400003c0 in test_backtrace () at test_backtrace_malloc.c:52
#4  0x0000000040000468 in main () at test_backtrace_malloc.c:160
#5  0x000000004000246c in _cstart () at reference/cstart.c:23
#6  0x0000000040000020 in _start_gdb ()
(gdb) print $fp
$1 = (void *) 0x4fffffb0
(gdb) p $sp
$2 = (void *) 0x4fffff50
(gdb) p $ra
$3 = (void (*)()) 0x400002fc <function_A+20>
(gdb) p *(void **)$fp
$4 = (void *) 0x4fffffd0
(gdb) x/32wx $sp
0x4fffff50: 0xffffffff  0xffffffff  0x4ffffafd  0x00000000
0x4fffff60: 0x00000000  0x00000000  0x00000000  0x00000000
0x4fffff70: 0x400002fc  0x00000000  0x40003820  0x00000000
0x4fffff80: 0x4fffffd0  0x00000000  0x00000000  0x00000000
0x4fffff90: 0x00000000  0x00000000  0x00000000  0x00000000
0x4fffffa0: 0x4fffffc0  0x00000000  0x400002fc  0x00000000
0x4fffffb0: 0x4fffffd0  0x00000000  0x40000320  0x00000000
0x4fffffc0: 0x4fffffe0  0x00000000  0x400003c0  0x00000000
```
- One piece of advice that cannot overemphasized when writing this code is that __pointer arithmetic scales by the size of the pointee type__. The expression `ptr + 2` when `ptr` is type `char *` is simply adding 2 to the base address `ptr`. However, if `ptr` is type `int *`, the expression  `ptr + 2` is adding 8 (2*sizeof(int)) to `ptr`. This will come up when using offset arithmetic to access relative locations within the stack. A location that is an offset of 12 bytes from a `char *` base can alternatively be computed as an offset of 3 words from a `int *` base, so be sure to consider pointee type when determining appropriate value for offset.

#### Printing a stack frame
After you have gathered the stack frames into an array, you print the array in order. Each frame is printed as a single line using this format:

```console?prompt=(gdb)
#0 0x400002fc at <.text+764>
#1 0x40000320 at <.text+800>
#2 0x400003c0 at <.text+960>
#3 0x40000468 at <.text+1128>
#4 0x4000246c at <.text+9324>
#5 0x40000020 at <.text+32>
```

The information shown within angle brackets is the "label" for each resume address. Ideally the label would identify the function name and offset. However in an executing program, functions are only referred by address, not name -- the symbol names are not even present in the `.bin` file 😢. (However, you can add symbol names as an [extension](#symbol-table)!)  The module `symtab.c` contains the function `symtab_label_for_addr` that provides the label to use in the basic case. This label is simply the offset of the address within the `.text` segment.

All stack frames use the same layout, so once your code can correctly backtrace one stack, that code is quite likely to correctly handle all other stacks without additional special cases. Confirm with a few tests and you're done with this module!

### Malloc module

The job of the heap allocator is to service allocation requests and manage the pool of available space.

Start by reviewing the header file (available as `$CS107E/include/malloc.h` or [browse malloc.h here](/header#malloc)). The functions exported by the `malloc` module are:

- `void *malloc(size_t nbytes)`
- `void free(void *ptr)`
- `void *sbrk(size_t nbytes)`

These functions operate in the same spirit as the same-named allocation 
functions of the standard C library. 

The essential requirement for any heap allocator is that it correctly service any combination of well-formed requests.  It is also desirable that it be efficient in use of space (recycling freed memory, compactly storing blocks) and efficient in use of cycles (quickly servicing requests). For the assignment, your primary goal will be to achieve correctness, with only a modest expectation on efficiency.

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
   
    + Trace the given code for `sbrk`. You will use this code as-is and it is important that you understand exactly what this function does for you and how to use it.
    + Where does the heap segment start in memory? Where does it end?  Review our linker script `$CS107E/lib/memmap.ld` to see how the symbols `__heap_start` and `__heap_max` are configured.
    + What is the return value of a call to `sbrk`? How does `sbrk` enlarge the available memory? What is the limit on how much it can grow and why is that limit needed? What happens if you attempt to advance `sbrk` beyond that limit?
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
   
   When adding the header, be sure to consider how it will affect payload alignment. Each payload must start on an address that is a multiple of 8. One convenient way to adhere to the alignment rule is to round up each payload to a size that is a multiple of 8, add an 8-byte header, and lay out blocks end to end beginning at an address that is a multiple of 8. Our standard `memmap.ld` linker script aligns the `__heap_start__` symbol to an 8-byte boundary.

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
Your primary testing vehicle is the test program `test_backtrace_malloc.c` that corresponds to the `make test` target. Add your test cases for backtrace and malloc here.  The starter code provides example tests to get you started but you will need to add more tests of your own to get comprehensive coverage for your modules. A portion of your assignment quality assessment is awarded based on the quality and breadth of the test cases you submit.

The `make run` target runs our `heap_workflow.c` application program to exercise your allocator on a simulated workflow.  This simulation is designed for stress testing and finishing step.   While still in active development, running the workflow can be more complicated than helpful; you are likely better off working with small, specific tests that you hand-constructed in the test program. But once your allocator is complete, the workflow is super handy to confirm it is working correctly in large stress test. You can also edit `heap_workflow.c` to tweak the simulation parameters to observe the performance in different scenarios.

## Extensions

For this assignment, we have two proposed extensions. Completing just one is sufficient to receive extension credit, but doing both is super awesome and will be worth additional bonus. Each extension teaches you some additional neat things (a deep dive into build process, learn more about ELF file format, explore how memory debugging tools work) and these upgrades to your modules make them more valuable to you going forward (human-readable backtrace plus protection against memory blunders, yay!). The two extensions together also complement each other very well. We highly encourage you to try at least one!

<A name="symbol-table"></A>
### 1) Symbol table
You can match a single hex address from your backtrace to its symbolic function name using the command `riscv64-unknown-elf-addr2line`:
```console
$ riscv64-unknown-elf-addr2line 0x04000063c -f -e test_backtrace_malloc.elf
function_A
../assign4/test_backtrace_malloc.c:30
```
What would be way cooler is if `symtab_label_for_addr` were able to provide a label with a symbolic name for every address when printing a backtrace.  For this extension, you'll do exactly that!

You can apply the same technique debuggers do to print out friendly names: read the debug information in the object file. The format of the debug information has changed over time as our programming languages have become increasingly more complex. When times were simpler, the common debug format was [STABS](https://www.sourceware.org/gdb/onlinedocs/stabs.html/#overview). A similar old-school approach you can use is to read the symbol information and names from  the `.symtab` and `.strtab` sections of the ELF file.

 The `.symtab` (symbol table) section lists all of the symbols with type, size, and address. The associated names for the symbols are stored in a companion `.strtab` (string table) section. It is possible to dump the hex bytes from a ELF section using `riscv64-unknown-elf-readelf` like so:

```console
$ riscv64-unknown-elf-readelf program.elf -x .symtab
$ riscv64-unknown-elf-readelf program.elf -x .strtab
```

There are a number of details you will need to investigate for yourself to do this extension and you will surely learn some interesting things in the process!
- What is the format of the symtab/strtab sections in the ELF that hold the symbol data?
- How can you harvest the symbol data from the ELF file as part of the build process?
- How might you send the symbol data to the Pi and where should place in the Pi's memory so that your symtab module can find it?
- How will you access the symbol data within your program running on Pi and use it provide labels to backtrace?

With access to the symbol data, you can implement the functions in `symtab.c` module to provide the friendly labels needed for a human-readable backtrace:

```console?prompt=(gdb)
#0 0x04000064 at <function_A+20>
#1 0x40000668 at <function_B+20>
#2 0x40000708 at <test_backtrace+32>
#3 0x40000b80 at <main+36>
#4 0x4000212c at <_cstart+60>
#5 0x40000020 at <_start+32>
```
Very neat!

(By the way, these fancy labels also tie in beautifully when decoding branch and jump instructions for the assign3 disassembler extension...)

Additional references on ELF:
- [ELF explanation](elf)  I prepared for a long-ago CS107 assignment
- [ELF 101](https://github.com/corkami/pics/blob/master/binary/ELF.png) ELF in one visual diagram
- [ELF osdev wiki](https://wiki.osdev.org/ELF)
- [Nice walkthrough on ELF](https://linux-audit.com/elf-binaries-on-linux-understanding-and-analysis/) on Linux Audit
- Full 100-page [ELF specification](https://archive.org/details/elf_20210512) for all you insomniacs

### 2) Mini-Valgrind
In this extension, you combine your backtrace and malloc modules to add a "Mini-Valgrind" set of memory debugging features to your heap allocator to report on memory errors and memory leaks.

What happens if a client allocates a block of memory and then mistakenly writes
outside of the bounds?  The erroneous write not only stomps on the neighboring payload data, it is almost certain to destroy the neighboring block header. A memory error that corrupts this critical heap housekeeping will lead to many sad consequences. 

What happens if a client allocates a block, but forgets to free it when done? This creates a _memory leak_, where the block is parked there forever, unable to be recycled. A memory leak is much less devastating than a memory error, but can degrade performance over time and eventually cause issues.

Memory debugging tools such as [Valgrind](http://valgrind.org) are invaluable weapons in the battle against difficult-to-find memory errors and memory leaks. For the extension, you are to implement a "Mini-Valgrind" that adds memory error and memory leak detection to your heap allocator.

> __This extension will require a somewhat invasive re-structuring of malloc/free.__ Before attempting the extension, commit, tag, and submit your completed code for the core features. This establishes a known working version that can serve as a point of comparison as you move forward and it may be your lifeline if you end up needing a known good place to restore to.
{: .callout-warning}

#### Red zones and leak detection

One technique for detecting memory over/underruns is to surround each payload with a pair of _red zones_. When servicing a malloc request, oversize the space by an extra 8 bytes. Place the actual payload in the middle with one 4-byte red zone before it and another 4-byte red zone after it. (Take care to keep the payload aligned on the 8-byte boundary). Fill the red zone with a distinctive repeating value (a good use for your handy `memset` function!). Our implementation uses `0x99`, though you are free to use [any non-offensive pattern you like](https://en.wikipedia.org/wiki/Hexspeak). When the client later frees that block, check the red zones and squawk loudly if the value has been perturbed.

In addition, you should tag each allocated block with the context it was allocated from by recording a "mini-backtrace". Modify your block header structure to add an array of three `frame_t` and fill with a snapshot of the innermost three frames at the point this block was allocated. 

Modify `free` to verify that the red zones are intact for the block currently being freed. If not, print an error message about the block in error along with its mini-backtrace that identifies where the block was allocated from.

```console?prompt=(gdb)
=============================================
 **********  Mini-Valgrind Alert  ********** 
=============================================
Attempt to free address 0x0000a978 that has damaged red zone(s): [45999999] [99999999]
Block of size 5 bytes, allocated by
#0 0x400000d4c at malloc+192
#1 0x400000650 at test_heap_redzones+52
#2 0x4000006bc at main+56
```

The values for the address, damaged red zone, size, etc. will be specific to the block in error, but please try to make
the rest of the error report use the same format and wording as our above sample. (Your graders thank you in advance for taking this care!)

Mini-Valgrind also tracks heap statistics, such as the count of calls to `malloc` and `free` and the aggregate total number of bytes requested. Implement the function `memory_report` that is intended to be used at program completion to print a summary of the total heap usage and list all remaining memory leaks. The mini-backtrace stored in the block header is used to identify the context for each leaked block.

```console?prompt=(gdb)
=============================================
         Mini-Valgrind Memory Report          
=============================================
final stats: 42 allocs, 40 frees, 5752 total bytes requested

8 bytes are lost, allocated by
#0 0x400000d0c at malloc+196
#1 0x400000674 at main+28
#2 0x4000006ec at _cstart+48

100 bytes are lost, allocated by
#0 0x400000d0c at malloc+196
#1 0x40000068c at main+52
#2 0x4000006ec at _cstart+48

Lost 108 total bytes in 2 blocks. 
```

Please take care to match the wording and format of our sample memory report in order to ease the graders's job.

As a final detail, work out where you can put a call to `memory_report()` so that Mini-Valgrind can provide leak detection to any program finishes, without having to modify each program's `main` function. Hint: think about where the Pi resets when your test program runs to successful completion. Make your own copy of that file, modify as needed, and edit the Makefile to build with your replacement version.

To submit the extension for grading, tag your completed code `assign4-extension`.

## Submitting

The deliverables for `assign4-submit` are:

- implementations of the `backtrace.c` and `malloc.c` library modules (`symtab.c` if you did the extension)
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
    - harvest stack frames
    - print frames
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
