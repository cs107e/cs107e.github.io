---
released: true
permalink: /assignments/assign4/
title: 'Assignment 4: Stack and Heap'
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
For this week's assignment, you continue in the role of library implementer to add a stack debugging aid (`backtrace.c`) and a heap allocator (`malloc.c`) to your growing collection of foundational routines.

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
- `backtrace.c`, `malloc.c`:  library modules
- `test_backtrace_malloc.c`:  test program with your unit tests for backtrace and malloc
- `heap_workflow.c`:  simulation application that runs a generated workflow against your heap allocator and reports on correctness and performance
- `extension.c`: Edit this file to implement extension (if you choose)
- `Makefile`: rules to build heap_workflow application (`make run`) and unit test program (`make test`). Also a `make debug` target that runs the unit test program under gdb.
- `README.md`: edit this text file to communicate with us about your submission
- `style_reflection.txt`: a text file that you will edit for your responses to the style reflection prompts

The `make run` target builds and runs the sample application `heap_workflow.bin`. This target is useful late in your development cycle as a final confirmation of your completed allocator. The `make test` target builds and run the test program `test_backtrace_malloc.bin`. This test program is where you will add your unit tests. You will make heavy use of this target early and often during development. You can also run the unit test program under the gdb debugger using the `make debug` target.

> __Reminder: using (or not) your previous modules__
> You can edit `MY_MODULE_SOURCES` in `Makefile` to choose which of your previous modules to build on. See instructions for [use of MY_MODULE_SOURCES](/assignments/assign3/#mymodules) in assignment 3.
{:.callout-info}

## Core functionality

### Backtrace module
The `backtrace` module provides functionality to gather and print a stack backtrace. A backtrace lists the current call stack beginning at the currently executing function, tracing backwards through the sequence of caller functions and ending at the program start.

The header file for the module is available as `$CS107E/include/backtrace.h` or [browse backtrace.h here](/header#backtrace). The public functions exported by the module are:

- `int backtrace_gather_frames(frame_t f[], int max_frames)`
- `void backtrace_print_frames(frame_t f[], int n)`
- `void backtrace_print(void)`

The two `backtrace_print` functions are given to you pre-written; your job is to implement `backtrace_gather_frames`. This function harvests the sequence of `saved ra` values from the stack.

#### Harvesting stack frames
Before writing any code, we recommend that you confirm your understanding of the stack frame layout by poking around in `gdb`, exploring the runtime state of an executing program, stepping through function calls, and examining the stack memory and register values. Deciphering the [stack mystery exercise in Lab 4](/labs/lab4/#solving-a-stack-mystery) using pen and paper can be a helpful starting point to verify your understanding.

The `backtrace_gather_frames` function fills an array of `frame_t`, one for each function on the stack. The first entry in the array is filled with the frame corresponding for the topmost function, the subsequent entry is the caller of that function and so on working backwards through the call stack. A backtrace stops at the outermost function `_start`.

The current value of the `fp` register is the "anchor" you need to start examining the stack. You must drop down to assembly to access a register. The GCC `__asm__` keyword allows you to mix assembly in with your C code. The code sample below demonstrates using embedded assembly to store the value of the `fp` register into a variable
named `fp_reg`:

```c
void *fp_reg;
__asm__("mv %0, fp" : "=r" (fp_reg));
```

> Gcc inline assembly has some goopy syntax; don't feel you need to make a deep dive into it. Copy the above snippet to accesss the value of the fp register. If you're
curious to learn more, see gcc manual
[support for inline assembly](https://gcc.gnu.org/onlinedocs/gcc/Using-Assembly-Language-with-C.html).
{: .callout-info}

It may help to have your stack diagram from lab4 handy as you consider the following thought questions.

- The frame for the currently executing function is topmost on the stack.  After executing the function prolog, where on the stack does the `fp` register point?  At what offset from that location can you find the `saved ra`? the `saved fp`?
- What does the value of the `saved ra` tell you about the caller of the current function?
- How can the `saved fp` be used to access to the stack frame for the caller?  How do you then go from that frame to the frame of its caller and so on? What is the indication that you have hit the outermost stack frame? This is where backtrace stops.
- Confirm your answers to the above questions by running your test program under the debugger, set a breakpoint at the point you want to backtrace, and then when stopped at the breakpoint, use gdb commands to print registers and examine memory.  Validating your backtrace against `gdb`'s `backtrace` command is another useful debugging technique.
```console?prompt=(gdb)
Breakpoint 2, recursion (n=n@entry=1) at test_backtrace_malloc.c:16
16  static int recursion(int n) {
(gdb) backtrace
#0  recursion (n=n@entry=1) at test_backtrace_malloc.c:16
#1  0x000000004000023c in recursion (n=n@entry=2) at test_backtrace_malloc.c:22
#2  0x000000004000024c in recursion (n=n@entry=3) at test_backtrace_malloc.c:24
#3  0x000000004000023c in recursion (n=n@entry=4) at test_backtrace_malloc.c:22
#4  0x0000000040000288 in test_backtrace () at test_backtrace_malloc.c:51
#5  0x0000000040000794 in main () at test_backtrace_malloc.c:208
#6  0x0000000040005d90 in _cstart () at reference/cstart.c:21
#7  0x0000000040000020 in _start ()
(gdb) print $fp
$1 = (void *) 0x4fffff90
(gdb) p $sp
$2 = (void *) 0x4fffff70
(gdb) p $ra
$3 = (void (*)()) 0x4000023c <recursion+64>
(gdb) x/32wx $sp
0x4fffff70: 0x00000000  0x00000000  0x00000003  0x00000000
0x4fffff80: 0x4fffffb0  0x00000000  0x4000024c  0x00000000
0x4fffff90: 0x00000000  0x00000000  0x00000004  0x00000000
0x4fffffa0: 0x4fffffd0  0x00000000  0x4000023c  0x00000000
0x4fffffb0: 0x4fffffd0  0x00000000  0x00000000  0x00000000
0x4fffffc0: 0x4fffffe0  0x00000000  0x40000288  0x00000000
0x4fffffd0: 0x4ffffff0  0x00000000  0x40000794  0x00000000
0x4fffffe0: 0x50000000  0x00000000  0x40005d90  0x00000000
```
- One piece of advice that cannot overemphasized when writing this code is that __pointer arithmetic always scales by the size of the pointee type__. The expression `ptr + 2` when `ptr` is type `char *` adds 2 because the pointee type of `char` is 1 byte (no additional scaling). However, if `ptr` is type `int *`, the expression  `ptr + 2` adds 8 (= `2 * sizeof(int)` ) because the pointee type `int` is 4 bytes. This will come up when using offset arithmetic to access relative locations within the stack. A location that is an offset of 8 bytes from a `char *` base can alternatively be computed as an offset of 1 doubleword from a `void **` base. Be sure to consider pointee type when determining appropriate value for offset.

All stack frames use the same layout, so once your code can correctly gather frames from one stack, that code is quite likely to correctly handle all other stacks as well. Unifying on a single code path for all cases is not only good design but reduces the testing burden.

#### Printing a backtrace
After you have gathered the stack frames, the `backtrace_print_frames` function is used to print them out. Frames are printed one per line in the format below:

```console?prompt=(gdb)
#0 0x40000228 at <recursion+44>
#1 0x4000023c at <recursion+64>
#2 0x4000024c at <recursion+80>
#3 0x4000023c at <recursion+64>
#4 0x40000288 at <test_backtrace+28>
#5 0x40000794 at <main+36>
#6 0x40005d90 at <_cstart+60>
#7 0x40000020 at <_start+32>
```

The information in angle brackets is the __label__ which identifies the function that contains this address and the offset of how far into the function it occurs. Finding the function name for an address needs a bit of support magic we provide. In an executing program, functions are only referred by address, not name. Symbol names are not even present in the `.bin` file! However, they are in the `.elf` file, and we can use the same technique as debuggers do: read names from the ELF symbol table used by the linker.

 The data is found in the `.symtab` and `.strtab` sections of the ELF file. If you are curious to see innards for yourself, try out`

```console
$ riscv64-unknown-elf-readelf -s program.elf
```

Read the `symtab` header file (available as `$CS107E/include/symtab.h` or [browse symtab.h here](/header#symtab)) to review the available features.  The backtrace module uses the `symtab` module as a client, you will not need to edit the symtab implementation.  `mango-run` uses `xfel write` to store the ELF data to a certain memory location (set in `memmap.ld`); this makes the data accessible to the `symtab` module. If symbol names are not available, symbol prepares a label that just the offset of the address within the `.text` segment.

(Side note: The symtab label adds a nice touch of polish to identify the target of a disassembled branch or jump instruction if you did that extension for assign3)

#### Stack protector
The final feature of your `backtrace` module is to enable detection and reporting of overflow on stack-allocated buffer variables.

When a C function declares an array on the stack, we refer to this variable as a "stack buffer". When buggy code erroneously writes past the end of the allocated buffer size, this error is called a "buffer overflow" or if overflowing a stack buffer, "stack overflow".  An overflow error silently overwrites the neighboring data in memory, and the observed consequences are often very perplexing and difficult to track down.  Stack overflows are particularly deadly as the adjacent data on the stack is critical information to the executing program, e.g. saved registers and saved return addresses. If that data is corrupted, it can lead to program crashes, incorrect operation, and security issues.

The gcc compiler has a feature called __StackGuard__ that helps with detecting stack buffer overflows. StackGuard is enabled by the compiler flag `-fstack-protector-strong`. When enabled, StackGuard modifies the layout of a stack frame to place a "canary" after the end of a stack buffer and before the neighboring stack housekeeping data.  The compiler will generate additional instructions in the function prolog that write the canary value into the stack frame and instructions in the epilog verify that the canary value has not be perturbed. If the canary value has been changed, it assumes the buffer has been overflowed and immediately transfers control to an error-handling function to halt the program, thus preventing further misbehavior or stopping the attempt of an attacker to take control over the program.

Here is a CompilerExplorer link <https://gcc.godbolt.org/z/MqEMMf6cc> that compares the assembly for an ordinary compile to the assembly generated when StackGuard is enabled.  Carefully trace the assembly and sketch a stack diagram to see how StackGuard works. StackGuard places a canary in the stack frame of any function that declares a stack buffer, other functions do not require protection.

StackGuard relies on two global symbols, `__stack_chk_guard` (a data symbol containing the value to use for canary) and `__stack_chk_fail` (a function called when overflow is detected). These symbols are provided in the standard library, but given we run bare-metal, we must write them for ourselves.  The starter code in `backtrace.c` provides starting definitions that you will finish off. Set the `__stack_chk_guard `  canary to the value of your choice, and finish the code for `__stack_chk_fail` to determine which function was responsible for the stack buffer overflow and report with a message that uses this wording and format:

```console
*** Stack smashing detected at end of function bad_guy() ***
```

In the above example, the function `bad_guy()` is in error, it overflowed a stack buffer which overwrote the canary.  In the epilog of `bad_guy()`, the change in canary value triggers a call to `__stack_chk_fail`.  The stack frame for `bad_guy()` is compromised and the corruption may extend to the caller's frame and beyond.  At this point, we have to assume that the entirety of the stack leading up to `bad_guy()` is suspect. The call to `__stack_chk_fail` adds a new stack frame and this tippy-top part of the stack is the only part that we know to be valid.  In this dicey context, you should not attempt a call to the full version of `gather_backtrace`, instead you will extract just the `saved ra` from the topmost frame.

To report which function was responsible for the stack smashing, you need to match that `saved ra` to its symbol.  However, this `saved ra` is a little bit different than a usual call due to the declaration of `__stack_chk_fail` as attribute `noreturn`.

```
void __stack_chk_fail(void) __attribute__ ((noreturn));
```

The `noreturn` attribute indicates that once a call is made to `__stack_chk_fail` it does not return to the caller; program execution stops within the call. In this situation, this is the only right thing to do. The caller is the bad actor who has corrupted the stack and the compiler absolutely does not want to return control to this caller and unleash further havoc.

When generating the call to a `noreturn` function, the compiler knows that control will not return to the caller.  Revisit the Compiler Explorer link <https://gcc.godbolt.org/z/MqEMMf6cc> and disassemble in gdb and look very carefully at where the compiler has placed the instruction `call __stack_chk_fail` relative to the rest of the function instructions and what that will mean for the value of the return address recorded by the `call` (keep in mind `call` is a pseudoinstruction for `jal`). You can read the `saved ra` from the stack as usual, but the steps to match that `saved ra` to the proper function symbol name will need to be a bit different.  It may help to draw a stack diagram and single-step through the disassembly in gdb to sort out what is going on. Working out how to interpret that ra and correctly identify the bad actor is a great flex of your stack-wrangling and RISC-V disassembly superpowers!

Edit your Makefile to add the flag `-fstack-protector-strong` to `CFLAGS` and do a `make clean` and `make`.

> __Pro-tip: make clean__ Whenever you make a change to a Makefile, always follow up with `make clean`. You want to ensure your next `make` will start fresh and apply the updated build settings to all files. `make clean` removes all previous build products, thus ensuring a fresh recompile of all files. Without `make clean`, you could end up with a mishmash of files compiled under the old and new settings.
{: .callout-warning }

The buggy `bad_guy` function that previously got away with its nefarious doings should now be stopped in its tracks by your stack protection. Neat!  It will be wise to keep this protection enabled from here forward. Following up on alert from stack protector is way more direct path to finding and fixing the bug than trying to decipher a mysterious crash.

Neat references for further learning:
-  Good overview of StackGuard from RedHat <https://www.redhat.com/en/blog/security-technologies-stack-smashing-protection-stackguard>
    - Includes interesting considerations for choosing the canary value
- <https://wiki.osdev.org/Stack_Smashing_Protector>
    - Good discussion what to anticipate from would-be malicious actors in detection handler
- Famous anonymous essay "Smashing The Stack For Fun And Profit" <https://phrack.org/issues/49/14.html> that spawned the creation for StackGuard as countermeasure.


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

1. __Study starter code.__

    The file `malloc.c` in the starter code contains the super simple "bump allocator" shown in lecture. Start by reviewing this code. The bump allocator is correct, but quite naive. It serves as a reference point from which your own heap allocator will be able to show substantial improvements.
   
    Make sure you understand what each line in the bump allocator is doing. Your allocator will be significantly more complicated, so it is best to start from a solid foundation. Here are some self-test questions to work through to verify your understanding:
   
    - Trace the given code for `sbrk`. You will use this code as-is and it is important that you understand exactly what this function does for you and how to use it.
    - Where does the heap segment start in memory? Where does it end?  Review our linker script [`$CS107E/lib/memmap.ld`](/src#memmap) to see where the markers `__heap_start` and `__heap_max` are placed and the corresponding constants `HEAP_START` and `HEAP_MAX` defined in [`$CS107E/include/memmap.h`](/header#memmap)
    - What is the return value of a call to `sbrk`? How does `sbrk` enlarge the available memory? What is the limit on how much it can grow and why is that limit needed? What happens if you attempt to advance `sbrk` beyond that limit?
    - The given code casts the `void *` pointer to `(char *)` before doing pointer arithmetic. What does this typecast do and why is it needed?
    - When servicing a request for `malloc(5)`, how much space is actually reserved by the bump allocator?
    - Every allocation is required to be 8-byte aligned (i.e. payload must start on an address which is a multiple of 8). How does the bump allocator achieve this requirement?
    - Study the `roundup` operation. Plug in a few sizes and trace how it operates. Why does the bitwise trick only work for multiples of numbers that are powers of 2?
    - Why is the bump allocator unable to recycle a previous allocation that has been freed?

    The `make run` target runs the simulation program `heap_workflow.c` that processes a randomly generated sequence of requests to malloc and free. The simulation confirms that the allocator correctly services the requests and reports on its performance. Review the code in `heap_workflow.c` to see how the simulator operates. The general gist is that the simulation program tracks the in-use blocks and confirms the blocks are properly managed. It also times the operations (using your handy timer module) and tracks the growth of the bounds of heap. At completion of the simulation, it reports the measured time and space efficiency.

    The `heap_workflow.c` simulation program is ~250 lines, about a third longer than our reference malloc implementation. Sometimes it takes more code to test a module than it did to write it in the first place! Our simulation program will be useful as a final check on your working allocator.

    Use `make run` right now on the starter code to see the bump allocator in action. The bump allocator is correct but lazy. It process requests very quickly but remorselessly chews through memory with no hope of recycling. You can do better!

2. __Re-work `malloc()` to add a block header to each block.__
    
   Your heap allocator will place a header on each block that tracks the block size and status (in-use or free).  Review these [block header diagrams](block_headers/) to understand the basic layout of a heap using block headers.
   
   The bump allocator services a `malloc` request by extending from the break to place the new block at the previous heap end. Change malloc to also include additional space for a block header and fill that header with the block status and size.
   
   When adding the header, be sure to consider how it will affect payload alignment. Each payload must start on an address that is a multiple of 8. One convenient way to adhere to the alignment rule is to round up each payload to a size that is a multiple of 8, add an 8-byte header, and lay out blocks end to end beginning at an address that is a multiple of 8. Our standard `memmap.ld` linker script aligns the `__heap_start` marker to an 8-byte boundary.

3. __Implement `free()` to update status in block header.__
   
   `free()` should access the block's header and mark it as free. Accessing the neighboring memory at a negative offset via pointer subtraction works just like pointer addition -- handy!
   
   This doesn't yet recycle a free block, but marking it as free is the first step toward that.

4. __Implement `heap_dump()` to output debugging information.__
   
   Before things get any more complex, implement a routine to give you insight into what's going on inside your heap.  The intention of `heap_dump()` is to print a visual dump of your heap contents that will help you when debugging your allocator.  Yay `printf`, where have you been all my life?!  
   
   As a starting point, we suggest that `heap_dump` walk the heap and print each block. Starting from the first block, traverse from block to block until you reach the heap end. You can move from a block header to its forward neighbor by using pointer arithmetic to advance by the block size. The block headers are said to form an _implicit list_.
   
   For each block, print the payload address and size and status. You might also elect to print the payload data. If so, we recommend abbreviating by perhaps displaying only the first 16 bytes. If you instead print out every single byte, the output can become overwhelming for larger heaps. 
   
   Use the `test_heap_dump()` test in `test_backtrace_malloc.c` to try out `heap_dump`. You can add calls to `heap_dump()` wherever desired to get a visual dump of your heap contents for debugging. If you inspect the dump and see that your heap is in a corrupted state, add calls to dump earlier in the sequence and try to pinpoint the exact operation where your heap went from good to bad. Having an inside view of what's going on inside your heap is an invaluable aid for debugging.

   What you print in `heap_dump` will not be graded. This debugging function is yours to use as you see fit, but the more help it provides to you, the better off you will be. 

5. __Upgrade `malloc()` to search freed blocks and reuse/split.__
      
   Now change `malloc` to recycle a free block if possible. Walk the heap by traversing the implicit list of headers, and look for a free block of an appropriate size. A block that exactly matched the requested size would be ideal, but searching the entire heap to find that best fit block can be time-consuming. A quicker approach is to go with _first fit_, i.e. grab the first block found that is large enough. If this block satisfies the request with enough excess leftover to form a non-empty second block, split that remainder into its own free block.
   
   If no recyclable block is found, then just extend the heap and place the new block at the end as before.
   
   Add your own `test_recycle` case to `test_backtrace_malloc.c` to test that your allocator will now recycle freed memory. For example, if you run a loop that repeatedly does malloc-free (same size malloc each time), the same block should be repeated re-used.  Now add a `test_split` case that does a large-size malloc and frees it, followed by several small-size mallocs to see the splitting of the previous large block.  Make liberal use of `heap_dump` to confirm the internal configuration of your allocator during these operations is as you intend. 

6. __Upgrade `free()` to coalesce adjacent free blocks.__
   
   Reusing single freed blocks is a good improvement, but there is further opportunity for recycling. What happens if the client repeatedly allocates 8-byte blocks until the heap is full, frees all of them, then tries to allocate a 32-byte block? The heap contains much more than 32 total bytes of free space, but it has been fragmented  into individual 8-byte blocks.  To service a larger request, we'll need to join those smaller blocks.
   
   Modify `free()` so that when freeing a block, it checks if the block's forward neighbor is also free, and if so, it will _coalesce_ the two blocks into one combined free block. It then considers the forward neighbor of that bigger block to see if it's free and can also be coalesced. Continue forward-coalescing until no further coalescing is possible. Coalescing is only applied in the forward direction as our design does not support efficient access to the backward neighbor.

   Add a `test_coalesce` case to `test_backtrace_malloc.c` that runs a sequence designed for forward coalesce, such as request several small blocks, free the blocks in reverse order, and then request a large block.  Use `heap_dump` to confirm the coalescing and re-use of previous blocks.


7. __Watch your heap at work!__

    After confirmed your allocator's correct behavior on your own test cases, you're ready for a final spin using the simulation application program `heap_workflow.c`. Use the `make run` target to run the simulation. The simulation runs a long random sequence of malloc/free calls, confirming that each request is correctly serviced and payload data is properly managed. It also measures the time spent servicing each request and observed the growth in the footprint of the heap segment. At the end of the simulation, it reports summary statistics on the allocator performance. Compare your results to the original results of the lazy bump allocator and take a bow for your hard work showing up that slacker!


__Congratulations!__ You have reached the summit of Heap Allocator Mountain ⛰ and earned your Pointer Wrangling Merit Badge 🏆 Time to celebrate with a nice cup of tea ☕️, a moment of mediation 🧘‍♀️, or a nap in the warm sun ☀️.

## Testing <a name="testing"></a>
Your primary testing vehicle is the test program `test_backtrace_malloc.c` that corresponds to the `make test` target. Add your test cases for backtrace and malloc here.  The starter code provides example tests to get you started but you will need to add more tests of your own to get comprehensive coverage for your modules. The grader will review the quality and breadth of the test cases you submit as part of the quality assessment.

The `make run` target runs our `heap_workflow.c` application program to exercise your allocator on a simulated workflow.  This simulation is designed for stress testing and finishing step.   While still in active development, running the workflow can be more complicated than helpful; you are likely better off working with small, specific tests that you hand-constructed in the test program. But once your allocator is complete, the workflow is super handy to confirm it is working correctly in large stress test. You can also edit `heap_workflow.c` to tweak the simulation parameters to observe the performance in different scenarios.

## Extension
There is a lot of memory wrangling that goes into a bare-metal program, from the code/data segments laid out by the linker to the managment of the runtime stack and the malloc module code to support dynamic allocation.  Having a solid understanding of how memory is organized and managed is an important foundation for the systems programmer. If you're looking to further your mastery, here are some further adventures to embark upon:

- make your allocator full-featured, fast, compact
    - For embedded systems, `malloc` is a lightly-used enhancement, but in the hosted world, it is a critical workhorse that everything heavily depends on. Much active ongoing effort goes toward further tuning the peformance of the system malloc.
    - The final project of CS107 is a deep dive into heap allocators. A simple implicit list version is the warmup before the main event to implement an explicit list that has high throughput and tight utilization. It also supports a `realloc` operation to resize a malloc block. Review the assignment writeup to see what tasks you could take on to learn more about building an industrial-strength allocator:
        - [CS107 heap allocator assignment](https://web.stanford.edu/class/archive/cs/cs107/cs107.1264/assign6/)
- Now that you have dynamic allocation in your bag of tricks, put it to work!
    - `malloc = malloc + memset` <https://en.cppreference.com/c/memory/calloc>
    - `strdup/strndup = malloc + strcpy ` <https://en.cppreference.com/c/string/byte/strndup>
    - `asprintf = malloc + printf` <https://en.cppreference.com/c/experimental/dynamic/asprintf> (GNU extension)
    - `get_line = malloc + uart_getchar` <https://man7.org/linux/man-pages/man3/getline.3.html> (POSIX)
- make an instrumented allocator for help debugging
    - Tools such as [Valgrind](http://valgrind.org) are invaluable weapons in the battle against difficult-to-find memory problems. Valgrind can detect and report issues such as:
        - read/write invalid memory (outside payload bounds, accessing freed memory)
        - incorect use of free (double free, attempt to free invalid pointer e.g. not start of payload or outside heap entirely)
        - memory leaks (dynamic allocations that are never freed)
    - Various thoughts on how to approach some of the above:
        - Detect block over/underruns is to add _red zones_ that immediately abut each payload. Red zones are akin to the canary used by StackGuard. Fill zone with a distinctive pattern and if you later detect the contents have been corrupted, squawk loudly!
        - Find leaks by adding a heap scan at program end and reporting all unfreed blocks. Be sure to consider how to make the scan run automatically after main completes.
        - Use your backtrace module to record a "mini-backtrace" snapshot of the top stack frames leading up to each malloc and store that with the malloc block (perhaps in header). When detect a problem, print mini-backtrace to identify which block is compromised.
        - An allocator instrumented for debugging may include extra checks, store more information, change operational behavior, disable recycle, etc. Trying to combine both standard/debugging into one allocator gets messy, we recommend separating into two distinct implementations.
    - Implementing these protections not only teaches you more about memory management, you gain a super-charged malloc module that helps alert you to any future mishaps with dynamic allocation. A extra pair of eyes on these tricky problems will be most welcome!
- detecting other memory problems
    - detect if program overwrites read-only data and/or text section
        - compute checksum of entire section contents at program start, again at program end and report if not same?
    - instrument function calls using gcc
        - insert function to be called during every function prolog/epilog, could use to do function tracing or detect excessive stack growth, infinite recursion
        - gcc docs  <https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html#index-finstrument-functions>
    - learn about Google AddressSanitizer (ASan), MemorySanitizer(MSan), UndefinedBehaviorSanitizer (UBSan)
        - <https://github.com/google/sanitizers/wiki/AddressSanitizer>
        - tools using compiler instrumentation and support libraries to detect and report various runtime errors
        - well-supported on hosted systems, mainstream ISAs, but more limited outside that
        - what is state of port to bare-metal RISC-V? Is there something you could try out or help try to make progress? Investigate and report back!
- Learn about parsing ELF and implement your own `symtab` module
    - your backtrace uses the libmango `symtab` to match an address to a symbol name. Our `symtab` module works by searching the symbol table of the ELF executable. Implementing your own version to learn about how ELF files are constructed. Start here for some [background on ELF files](elf).
    - for reasons of simplicity and expedience (laziness?)  mango-run copies the entire ELF file to memory after loading bin file, but it would be more correct to copy only the symtab and strtab sections. Even better would be to modify the build process to stuff those sections into the bin file so only one thing to load. I would be delighted to have a pleasing way to do this!
- OR something else entirely, come by office hours to brainstorm your ideas with us


Edit `extension.c` for your extension program (may be either application or unit tests for extended features) and use `make extension` to compile and run. Use the same git workflow as core, i.e. edit on dev branch, regularly commit and push. Edit the `README.md` file with quick summary of what you did for the extension. Apply tag `assign4-extension` to submit when done.

The grading review of extensions is done in-person, you'll give us a demo and share about your learnings. See times for extension demo/grading on OH schedule. We are curious to hear what you have learned!

The same guidelines apply to the extension as to the core: __it is fine to use online resources and generative AI tools to answer your questions and help you learn new things, but not cool to use it to write or fix your code.__


## Style reflection

Here are the instructions for the [style reflection](style_reflection) to be submitted with this assignment.

## Submitting

The deliverables for `assign4-submit` are:

- implementations of the `backtrace.c` and `malloc.c` library modules
- unit tests for both modules in `test_backtrace_malloc.c`
- `README.md` (possibly empty)
- your responses to the prompt questions in `style_reflection.txt`
- all `assign4-submit` files commit/tag/push by due date or before end of grace period

Additional for `assign4-extension`:

- extension program `extension.c`
- `README.md` with information on your extension
- use tag `assign4-extension`, commit/tag/push files by end of grace period

## Grading

To grade this assignment, we will:

- Verify that your submission builds correctly, with no warnings. Clean build always!
- Run our automated tests on your `backtrace` and `malloc` modules
- Go over the test cases you added to `test_backtrace_malloc.c` and evaluate for thoughtfulness and completeness in coverage.
- Review your completed style reflection.

Our highest priority tests will focus on the core features for this assignment:

- Essential functionality of your library modules
  - backtrace
    - harvest stack frames
    - print frames
  - malloc
    - service any sequence of valid requests
    - recycle free blocks
    - split and coalesce free blocks

The additional tests of lower priority will examine less critical features, edge cases, and robustness.

## Julie's aside on heap alligators
At home, my ongoing chatter about "heap allocator" was misheard as "heap alligator" by my kids, who were alarmed that I would ask my students to wrestle with such fearsome beasts.

A family trip to Gatorland taught us that a __heap of alligators__ can actually
be quite cute and the adorable drawing from Jane Lange (talented CS107e alum/TA, and current MIT PhD student) demonstrates that a tamed heap alligator makes a lovely pet. May you, too, tame your heap alligator and triumph over the perils of dynamic allocation.

![](images/heap_of_alligators.jpg){: .zoom}
![](images/heapalligator.png){: .zoom}
