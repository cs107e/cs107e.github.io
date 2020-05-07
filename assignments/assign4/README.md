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
- [ ] Followup on issues from previous quarter postmortem (issue #214)

__To prep for assign4:__
- [ ] 

{% endcomment %}

*Written by Julie Zelenski*

{% include duedate.html n=4 %}

## Goals
For this week's assignment, you are again acting in the role of
library implementer to add a debugging aid (`backtrace.c`)
and a heap allocator (`malloc.c`) to your growing
collection of foundational routines. 

Global and local variables have been sufficient for our needs thus far, but 
to support more complex applications we'll need support for dynamic allocation 
Dynamic allocation (a heap) offers the programmer explicit and 
direct control over sizing and lifetime of storage. With this
power comes with the responsibility to properly allocate and
deallocate that memory. Using dynamic memory correctly and safely is a
challenge for the client; implementing a heap allocator correctly
and efficiently is a challenge for the implementer.

After you finish this assignment, you will:

- be able to dissect the runtime stack in an executing program,

- further understand C data structures and memory layout,

- have more experience managing pointers in C, and

- appreciate some of the complexity and tradeoffs in implementing a heap allocator.

Each new assignment also provides many opportunities to appreciate and value your
hard work in previous weeks. You'll come to love how much easier _your_ `printf` makes
your testing and debugging.  By the end of the quarter, almost every single 
instruction in your fully functioning console will be from code you have written
and know well: you'll understand your whole system from the ground up.

## Get starter files
Change to the `cs107e.github.io` repository in your `cs107e_home` and do a `git pull` to ensure your courseware files are up to date.

To get the assignment starter code, change to your local repository, fetch any changes from the remote and switch to the assignment basic branch:
```
$ cd ~/cs107e_home/assignments
$ git fetch origin
$ git checkout assign4-basic
```

You have the option of building on your own code as you move forward or switching out your modules for our library versions. Edit the setting `MY_MODULES` in the Makefile to control which modules of yours are used and which of ours. Verify that the files `gpio.c`, `timer.c`, `strings.c`, and `printf.c` in your assign4-basic branch are up-to-date versions of your modules fi you would like to use your implementations from previous assignments.  The `git merge` command can be used to incorporate changes from another branch. For example, if you are missing your code in `gpio.c` or `timer.c` from assignment 2, use `git merge assign2-basic` to merge those changes into your current branch (while you have the assign4-basic branch checked out).  If you continued work on `assign3-basic` after your assign4-basic branch was created, use `git merge assign3-basic` to merge those latest updates. Using your own modules now will put you on track to receive the full system integration bonus after completing assignment 7.

The starter project contains the files `backtrace.c` and
`malloc.c`, the sample application program `apps/heapclient.c`, and the test program `tests/test_backtrace_malloc.c`. 

You will edit `backtrace.c` and `malloc.c` to implement the required functions. You will also add many tests to `tests/test_backtrace_malloc.c`.  The program `apps/heapclient.c` is used unchanged as a sample application.

The `make install` target of the Makefile builds and runs the sample application `apps/heapclient.bin`. The `make test` target builds and runs the test program `tests/test_backtrace_malloc.bin`. With no argument, `make` will build both, but not run. You can use the debugger in simulator mode on the corresponding ELF file, e.g. `arm-none-eabi-gdb tests/test_backtrace_malloc.elf`.

## Basic section

### Backtrace module
The `backtrace` module provides functionality to gather and print a stack backtrace. A backtrace is a listing of the function call stack beginning at the currently executing function, showing the sequence of calls that led to this function, and ending at `_cstart`.

Consider a program whose `main` function calls `tokenize`, which then calls `strndup`. A backtrace initiated during `strndup` could look something like this: 
```
#0 0x8090 at strndup+40
#1 0x814c at tokenize+116
#3 0x8354 at main+24
#4 0x83c0 at _cstart+48
```

The functions you are to implement in `backtrace.c` are:

- `int backtrace(frame_t f[], int max_frames)`
- `void print_frames(frame_t f[], int n)`
- `void print_backtrace(void)`
- `const char *name_of(uintptr_t fn_start_addr)`


Review our [backtrace.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/backtrace.h) for module documentation.
The two `print_` functions are given to you pre-written. You are to implement the `name_of` and `backtrace` functions. `name_of` finds the name for a given function and  `backtrace` fills an array of stack frame information. These two functions are small but mighty. Completing them will demonstrate your understanding of the stack layout and show off your finest pointer skills.

Before writing any code for the backtrace module, we recommend that you first spend time poking around in gdb and exploring the runtime state of the program, examining stack memory, stepping through function calls, and manually accessing the stack frame data, and so on. Exercise 4 of [Lab 4](/labs/lab4/#4-stack) may be worth a second look.

#### An example backtrace

The `backtrace` function fills an array of structs, one `frame_t` for each function on the stack. Read
[backtrace.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/backtrace.h) 
to see the type definition of the struct `frame_t`, including the comments which explain the intent of the fields `resume_addr` and `resume_offset`.

The `backtrace` function starts at the innermost stack frame and traces to the outermost, harvesting the information for each stack frame and storing into a `frame_t`.

The current value of the `fp` register is the "anchor" you need to start examining the stack. You must drop down to assembly to access a register. The GCC `__asm__` keyword allows you to mix assembly in with 
your C code. The code sample below demonstrates using embedded assembly to store the value of the `fp` register into a variable named `cur_fp`:

   ```
   void *cur_fp;
   __asm__("mov %0, fp" : "=r" (cur_fp));
   ```

(If you're 
   curious to learn more, check out the manual: [gcc support for inline assembly](https://gcc.gnu.org/onlinedocs/gcc/Using-Assembly-Language-with-C.html).)

We are using the `code/simple` program from lab3/lab4 as our example below. Have the Lab 4
   [memory diagram for simple](/labs/lab4/images/stack_abs.html) handy when working through the following thought questions.

   - The executing function is `abs()`. What is the value of the `fp` register? Relative to the `fp`, where in the stack can you read the `resume_addr` for the caller of `abs()`?

- How can you work from the `fp` for `abs()` to access the saved `fp` of its caller, the `diff` function?  How does the saved `fp` grant you access to the stack frame for `diff`? How do you then go from `diff`'s frame to the frame of its caller, the `main` function? What is the indication that you have hit the outermost stack frame? This is where backtrace stops.

- The `resume_offset` is calculated by subtracting the address of the first instruction of the caller from the resume address in the caller. What information in the stack frame for `diff` helps you locate its first instruction? (Hint: may help to re-visit question 4 on the 
[lab4 check-in](/labs/lab4/checkin/) 
for some foreshadowing about this calculation.)

The final task in harvesting the backtrace is to associate the name of the function with each stack frame. Symbol names are usually not present in a binary file as functions are referred to by address, not name. However, there is a compiler option to retain function names in the binary to support developers (such as yourself)
writing debugging tools. The assignment Makefile has the flag `-mpoke-function-name` enabled in `CFLAGS`. This option tells
gcc to store a function's name with the compiled instructions, embedded in the memory preceding the function's first instruction. 

{% include callout.html type="danger" %}
The documentation below is taken from the gcc manual, but we have corrected a few mis-identified details. Please use this version rather than refer back to the original.

</div>

   > `-mpoke-function-name`
   >
   > Write the name of each function into the text section, directly
   > preceding the function prologue. The generated code is similar to
   > this:
   >
   > ```
   > t0
   >     .ascii "arm_poke_function_name", 0
   >     .align
   > t1
   >     .word 0xff000000 + (t1 - t0)
   > arm_poke_function_name
   >     mov     r12, sp
   >     push    {fp, r12, lr, pc}
   >     sub     fp, r12, #4
   > ```
   >
   > When performing a stack backtrace, code can inspect the word value at the
   > location immediately preceding the first instruction. If that value 
   > has the most significant 8 bits set, then we know that there is a
   > function name embedded preceding value and the name has
   > length `(value & ~0xff000000)`.

The `name_of` function is given the address of a function's first instruction and finds the embedded function name, if available. If the word preceding a function's first instruction in memory has `0xff` in its most significant byte, this indicates that the function has its name embedded. The name's length is stored in the lower three bytes of the word. Use that length to back up in memory to the start of the string. Neat! If you don't find the magic `0xff` in the right place, this indicates the function name is not available, in which case, `name_of` should return `"???"`.

The embedded function name is null-terminated and the terminator is
counted in the length. The `.align` directive rounds up the length to a 
multiple of 4; additional null characters are used as padding. 
A function named `binky` will have its name embedded 
as `binky\0\0\0` and length of 8. Given that the name is
stored with its null terminator, `name_of` can access the string in-place and simply refer to it by address without making a copy of of the string.

If our previous example program is re-compiled with the `-mpoke-function-name` flag, the text section will contain the function names embedded alongside the instructions, (see [memory diagram with embedded function names](images/diagram_with_poke.html)). Using this diagram as a guide, work out by what steps you could access a function's name if starting from the address for the function's first instruction. This is what needs to happen in `name_of`.

Put this all together and you have a module capable of harvesting a backtrace for the current stack, including function names. It requires surprisingly little code, but it's dense and heavy on the pointer arithmetic and typecasts. Carefully map out your approach, take it slow, and use gdb as you go to examine memory and print expressions to confirm your steps. Validating your backtrace against `gdb`'s `backtrace` command can also be a useful debugging technique.

The stack layout follows a rigid convention, so once you have code to correctly backtrace a stack, it is likely to work equally well for all other stacks without special cases. Confirm with some simple tests and you're ready to move on.

### Malloc module

The job of the heap allocator is to service allocation requests and manage the pool of available space.

The functions you are to implement in `malloc.c` are:

   - `void *malloc(size_t nbytes)`
   - `void free(void *ptr)`
   - `void *realloc(void *ptr, size_t new_size)`
 
The module documentation in [malloc.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/malloc.h) describes the functions. They operate in the same spirit as the same-named allocation functions of the standard C library. 

The essential requirement for any heap allocator is that it correctly service any combination of well-formed requests.  It is also desirable that it be space-efficient (recycling freed memory, compactly storing blocks) and fast (quickly servicing requests). For the assignment, your primary goal will be to achieve correctness, with a lesser priority on efficiency.

#### Malloc specification

Refer to the module documentation in [malloc.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/malloc.h) for the operational behavior of each function. Other than the non-negotiable stipulation to correctly service all valid requests, there are very few requirements to the specification itself. 

All pointers returned by `malloc` are required be aligned to 8 bytes, the size of the largest primitive type on our system. 

If `malloc` cannot fulfill an allocation request, it returns `NULL`.  Four valid, but oddball cases to consider are `malloc(0)`, `realloc(NULL, size)`,  `realloc(ptr, 0)` and `free(NULL)`.  The expectation for these edge cases is given in the malloc.h documentation.

If a client request is invalid, such as an attempt to `realloc` a
random pointer that was not obtained from `malloc` or `free`ing an already freed pointer, the behavior of the allocator is undefined. We will not test such invalid calls.

#### Order of attack
Below we offer our suggestions for a step-by-step plan  of the tasks before you. 


1. **Study starter code.**

    Start by reviewing the given code for the "bump allocator" in the file `malloc.c`.  This bump allocator is simple and correct, but fairly naive. 
It serves as a reference point 
from which your own
heap allocator will be able to show substantial improvements.


    Make sure you understand what each line in the bump allocator is
doing. Your allocator will be significantly more complicated, so it is best to start from a solid foundation. Here are some self-test questions to work through to verify your understanding:

    - Trace the given code for `sbrk`. You will use this code as-is and it is important that you understand exactly what this function does for you and how to use it. Where does the heap segment start in memory? Where does it end? How does it enlarge? How big can it grow? What is the result of attempting to grow beyond the available limit?
    - Why does the code cast pointers to `char *` before doing pointer arithmetic?
    - If you request a 5-byte allocation, how much space is actually reserved?
    - Every allocation is required to be 8-byte aligned (i.e. payload must start on an address which is a multiple of 8). How does the bump allocator achieve this requirement?
    - Why is the bump allocator unable to recycle a previous allocation that has been freed?
    - Review the documentation for the `realloc` operation. Review the realloc code in the bump allocator and confirm your understanding of how the code accomplishes resizing the block. The bump allocator never performs an in-place realloc. What is meant by an "in-place realloc" and how does that compare to what the bump allocator does instead?

2. **Re-work `malloc()` to add a block header to each block.**
    
    Your heap allocator will place a header on each block that tracks the block size and status (in-use or free).  Review these [block header diagrams](block_headers/) to understand the basic layout of a heap using block headers.

    The bump allocator services a `malloc` request by extending from the break to place the new block at the previous heap end. Continue with the same strategy for now, but augment the operation to add a block header on the block.

    When adding the header, be sure to consider how it will affect payload alignment. Each payload must start on an address that is a multiple of 8. One convenient way to adhere to the alignment rule is
    to round up each payload to a size that is a multiple of 8, add an 8-byte header, and lay out blocks end to end starting from an address that is a multiple of 8. Our standard `memmap` aligns the `__bss_end__` symbol to an 8-byte boundary.

3. **Implement `free()` to update status in block header.**

    `free()` should access the block's header and mark it as free. Accessing the neighboring memory at a negative offset via pointer subtraction works just like pointer addition -- handy!
    
    This doesn't yet recycle a free block, but marking it as free is the
    first step toward that.

4. **Implement `heap_dump()` to output debugging information.**

    Before things get any more complex, implement a routine to give you visibility on what's going on inside your heap.  The intention of `heap_dump()` is to print a visual dump of your heap contents that will help you when debugging your allocator.  Yay `printf`, where have you been all my life?!  

    As a starting point, we suggest that `heap_dump` walk the heap and print each block. Starting from the first block, traverse from block to block until you reach the heap end. You can move from a block header to its forward neighbor by using pointer arithmetic to advance by the block size. The block headers are said to form an _implicit list_.

    For each block, print the payload address and size and status. You might also elect to print the payload data, and if so, we recommend abbreviating, perhaps displaying only the first 16 bytes. If you instead print out every single byte, the output can become overwhelming for larger heaps. 

    Use the `trace_heap()` test in `tests/test_backtrace_malloc.c` to try out `heap_dump`. You can add calls to `heap_dump()` wherever desired to get a visual dump of your heap contents for debugging. If you inspect the dump and see that your heap is in a corrupted state, add calls to dump earlier in the sequence and try to pinpoint the exact operation where your heap went from good to bad. Having an insider view of what's going on inside your heap is an invaluable aid for debugging.

    What you print in `heap_dump` will not be graded, this debugging function is yours to use as you see fit, but the more help it provides to you, the better off you will be.

5. **Upgrade `malloc()` to search freed blocks and reuse/split.**
   
    Now change `malloc` to recycle a free block if possible. Walk the heap by traversing the implicit list of headers, and look for a free block of an appropriate size. A block that exactly matched the requested size would be ideal, but searching the entire heap to find that best fit block can be time-consuming. A quicker fix is to go with _first fit_, i.e. grab the first block found that is large enough. If this block satisfies the request with enough excess leftover to form a second block, split off that remainder into its own free block.

    If no recyclable block is found, then just extend the heap and place the new block at the end as before.

    Concoct a sequence of malloc/free calls that should permit reuse and confirm with `heap_dump` that your allocator is recycling freed memory appropriately.

6. **Upgrade `free()` to coalesce adjacent free blocks.**
   
    Reusing single freed blocks is a good improvement, but there is further opportunity for recycling. What happens if the client repeatedly allocates 8-byte blocks until the heap is full, frees all of them, then tries to allocate a 32-byte block? The heap contains way more than 32 bytes of free space, but it has been fragmented it into individual 8-byte blocks.  To service a larger request, we'll need to join those smaller blocks.

    Modify `free()` so that when freeing a block, it checks if the block's forward neighbor is also free, and if so, it will _coalesce_ the two
    blocks into one combined free block. It then considers the forward neighbor of
    that bigger block to see if it's free and can also be coalesced. Continue
    forward-coalescing until no further coalescing is possible.

    Confirm forward-coalesce with test sequences and use of `heap_dump`.

7. **Upgrade `realloc()` to operate in-place when possible.**

    The final feature is to make a smarter `realloc`. The version of `realloc` given in the starter code always moves the payload data from the original block to a new block. If possible, we'd like to avoid that (expensive) copying and leave the payload data where it is. If the neighboring space is free, `realloc` could instead subsume the forward neighbor(s) into the original block, thereby resizing "in-place". Modify `realloc()` to seize this opportunity to resize the block in-place and only move the payload data when it must.

    Another round of tests and checking your heap contents with `heap_dump` and you've good to go!


__Congratulations!__ You have summited Heap Allocator Mountain ⛰ and earned your Pointer Wrangling Merit Badge 🏆 Time to celebrate with a nice cup of tea ☕️, a moment of mediation 🧘‍♀️, or a decadent nap in the sun ☀️.




 <a name="testing"></a>
## Testing

The `apps/heapclient.c` program contains sample client code adapted from lab4 that uses `backtrace` and `malloc`. Use this program unchanged as a basic sanity test of your modules.

The `tests/test_backtrace_malloc.c` program is yours to fill with your comprehensive suite of test cases. The starter code provides some example tests to get you started. You will need to add many more tests to throughly exercise your heap allocator in a wide variety of situations. A portion of your assignment grade is awarded based on the quality and breadth of the test cases you submit.

In CS106B, you learned about a variety of dynamic data structures (linked lists, trees, hash tables) and used them in the implementation of classic abstract data types (vector, stack, queue, dictionary, set). Given your shiny new heap allocator, you can now bring all that wonderful data structure goodness to your Pi.  Porting some of that code would make excellent and rigorous test cases for your allocator and in the process, you would gain a library of useful ADTs -- win-win!

## Extension: Mini-Valgrind
The extension for this assignment is to combine your backtrace and malloc modules to add a "Mini-Valgrind" set of memory debugging features to your heap allocator to report on memory errors and memory leaks.

What happens if a client allocates a block of memory and then mistakenly writes outside of the bounds?  The erroneous write not only stomps on the neighboring payload data, it is almost certain to destroy the neighboring block header. A memory error that corrupts this critical heap housekeeping will lead to many sad consequences. 

What happens if a client allocates a block, but 
forgets to free it when done? This creates a _memory leak_,
where the block is parked there forever, unable to be recycled. A memory leak is much less devastating than a memory error, but can degrade performance over time and eventually cause issues.

Memory debugging tools such as [Valgrind](http://valgrind.org) are invaluable weapons in the battle against difficult-to-find memory errors and leaks. For the extension, you are to implement a "Mini-Valgrind" that adds memory error and leak detection to your heap allocator.

#### Red zones and leak detection

One technique for detecting memory over/underruns is to surround each payload with a pair of _red zones_. When servicing a malloc request, oversize the space by an extra 8 bytes. Place the actual payload in the middle with one 4-byte red zone before it and another 4-byte red zone after it. (Take care to keep the payload aligned on the 8-byte boundary). Fill the red zone with a distinctive value. Our implementation uses a repeated `0x7e`, though you are free to use [any distinctive non-offensive value you like](https://en.wikipedia.org/wiki/Hexspeak). When the client later frees that block, check the red zones and squawk loudly if the value has been perturbed.

In addition, you should tag each allocated block with the context it was allocated from by recording a "mini-backtrace". Modify your block header structure to add an array of three `frame_t` and fill with a snapshot of the innermost three frames at the point this block was allocated. 

Modify `free` to verify that the red zones are intact for the block currently being freed. If not, print an error message about the block in error along with its mini-backtrace that identifies where the block was allocated from.

```
=============================================
 **********  Mini-Valgrind Alert  ********** 
=============================================
Attempt to free address 0x0000a978 that has damaged red zone(s): [457e7e7e] [7e7e7e7e]
Block of size 5 bytes, allocated by
#0 0x8d4c at malloc+192
#1 0x8650 at test_heap_redzones+52
#2 0x86bc at main+56
```

Format your error message as shown in the sample above. The values for the address, damaged red zone, block size, etc. will differ based on the test case, but you should otherwise exactly match the wording and format so as to be compatible with our automated checker. 

Mini-Valgrind also tracks heap statistics, such as the count of calls to `malloc` and `free` and the aggregate total number of bytes requested. Implement the function `memory_report` that is intended to be used at program completion to print a summary of the total heap usage and list all remaining memory leaks. The mini-backtrace stored in the block header is used to identify the context for each leaked block.

```
=============================================
         Mini-Valgrind Memory Report         
=============================================

malloc/free: 42 allocs, 40 frees, 5752 bytes allocated

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
The format of your memory report should exactly match the wording and format of the sample report shown above.

As a final detail, work out where you can call `memory_report()` so that Mini-Valgrind can provide leak detection to any program, without modifying the application's
`main` or other application files? Hint: think about how the green LED 
turns on when your test program runs to successful completion.

## Submit
The deliverables for `assign4-basic` are:

  - Implementation of the `backtrace.c` and `malloc.c` modules
  - Your comprehensive tests for all backtrace and malloc functions in `tests/test_backtrace_malloc.c`

Submit the finished version of your assignment by making a git “pull request”. Make separate pull requests for your basic and extension submissions.

Be sure to check that your submission:

- does not edit `src/apps/heapclient`. You should not need to make any edits to this file

- `make` and `make test` build successfully (and there are no warnings or errors!)


## Grading

To grade this assignment, we will:

- Verify that your submission builds correctly, with no warnings. Warnings and/or build errors result in automatic deductions. Clean build always!
- Run automated tests that exercise the functionality of your `backtrace.c` and `malloc.c` modules. These tests will touch on all required features of the module. 
- Go over the tests you added to `tests/test_backtrace_malloc.c` and evaluate them for thoughtfulness and completeness in coverage.
- Review your code and provide feedback on your design and style choices.

## Heap alligators
A note from Julie Zelenski, CS Lecturer, featuring a drawing from our very own Jane :D

At home, my chatter about "heap allocators" was mistaken for "heap alligators" by my kids, who were alarmed that I would ask my students to wrestle with such fearsome beasts. 

A family trip to Gatorland taught us that a heap __of__ alligators can actually be quite cute (see photo) and the adorable drawing from Jane Lange (talented CS107e alum and undergrad TA) demonstrates that a tamed heap alligator makes a lovely pet. May you, too, tame your heap alligator and triumph over the perils of dynamic allocation.

[<img title="Heap of Alligators" src="images/heap_of_alligators.jpg" width="50%" style="display:inline;">](images/heap_of_alligators.jpg)[<img title="Heap Alligator" src="images/heapalligator.png" width="50%" style="float:right;">](images/heapalligator.png)



