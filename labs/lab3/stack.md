---
title: 'Stack frames'
---

### Stack intuition
Functions often need space. For example, they allocate 
variables or store the return
address of their caller before calling another function. There's
nothing special about this space, and we could allocate it as we would
any other memory.  However, functions calls are frequent, so we want
them as fast as possible.  Fortunately, function calls have two important
properties we can exploit for speed:

1. When functions return, all allocated memory is no longer usable.
2. Functions return in Last-In-First-Out (LIFO) order.

To optimize for speed, people have converged on using a contiguous region of memory
(called a *stack* because it's LIFO, like a stack of plates).  Roughly speaking it
works as follows:

+ At program start we allocate a fixed-sized region to hold the stack and set a
pointer (the stack pointer) to the start of that region.
+ The memory the function needs is allocated contiguously by simply adjusting
the stack pointer.
+ When a function returns, it frees the allocated memory
all-at-once by returning the stack pointer to its position when the function
was called.

Note that the stack on our system operates as a _descending_
stack. This means the stack pointer is initialized to at the highest
address.  Code decrements the stack pointer to make space for
a function call and increments the stack pointer when that function
call returns. It looks like this:

![stack picture](../images/stack.png)

This organization is so effective that compilers explicitly
support it (and do the stack pointer increment and decrement) and
architecture manuals provide the rules for how to do so.

### APCS full frame stack layout
The executing function access the stack memory using relative offsets from the `sp` register and uses `push` and `pop` operations to store and retrieve values to and from the stack. 

A value being saved may be a caller-owned register or intermediate result. Large local variables (e.g. arrays and structs) that are too large to store in a register are also stored on the stack, but are more typically accommodated by adjusting the `sp` up and down by the needed amount than a sequence of push/pop instructions. All data stored on the stack by a function is collectively referred to its "stack frame". The compiler has latitude in what it stores on the stack; there is no guarantee about what values are stored in each frame and how values are laid out. This makes it challenging if trying to ascertain where each stack frame starts and stops.

Telling the compiler to use the APCS "full frame" variant enforces some order on the stack frame layout. The first four values pushed by a function are required to be the values of the four registers `fp`, `sp`, `lr`, and `pc`. These four saved registers form the "full frame". In addition, the register `r11` is used a dedicated "frame pointer" (`fp`). The `fp` is set to point to the first word pushed in the current stack frame (contrast to `sp` which points to the last word pushed). The `fp` tells you where to access the full frame of the currently executing function. At the end of Friday's lecture slides, there is a [stack diagram](/lectures/C_Functions/slides.pdf#page=22), showing the 4 saved registers that mark the beginning of each stack frame.

Reading the saved fp within the full frame tells you where to access the full frame of the caller. Following the sequence of fp links will lead back to the main function. This information is used by the debugger to produce a backtrace of the current call stack.  



