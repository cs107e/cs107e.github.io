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

Note that the stack on our system grows downward.  When making a function call,
the data for the callee function is stored at a lower address than the caller's data.
It looks like this:

![stack picture](../images/stack.png)

This organization is so effective that compilers explicitly
support it (and do the stack pointer increment and decrement) and
architecture manuals provide the rules for how to do so.

### Managing a stack on ARM architecture

Register `r13` (synonym `sp`) is designated for use as the stack pointer. 
`sp` points to the location of the last value added to the stack. This value is
referred to as the "top" of the stack. (yes, it is confusing that the top is stored at the lowest address!)

The stack operates as a _descending_
stack. This stack pointer is initialized to its highest
address. Adding a new value to the stack moves the `sp` downward (to a lower address).
Removing a value from the stack is accomplished by incrementing the `sp`.

The `push` instruction stores a value to the stack. 
For each named register, it copies the value to top of stack and decrements `sp`. 
```
push {r0, r4}
```

Values can be removed from top of the stack by manually adjusting the `sp`:
```
add sp, sp, #8
```
Alternatively, the `pop` instruction will copy values from the top of the stack into the named registers
and increment `sp` to remove them:
```
pop {r0, r4}
```

`push` and `pop` are aliases for the instructions `ldm` and `stm` (load and store multiple).  The multiple operations
read/write a group of registers to consecutive locations in memory, which makes them ideal for moving values
on/off the stack. There is a helpful explanation of `ldm` with further examples at <https://keleshev.com/ldm-my-favorite-arm-instruction/>

Typical values pushed to the stack are a caller-owned register or intermediate results. Large local variables (e.g. arrays and structs) that are too large to store in a register are also stored on the stack. These are generally accommodated by adjusting the `sp` by the needed amount than a sequence of push/pop instructions.

### APCS full frame stack layout
The executing function access the values on the stack using relative offsets from the `sp` register.  All data stored on the stack by a function is collectively referred to its "stack frame". The compiler has latitude in what it stores on the stack; there is no guarantee about what values are stored in each frame and how values are laid out. This makes it challenging if trying to ascertain where each stack frame starts and stops.

Telling the compiler to use the APCS "full frame" variant enforces some order on the stack frame layout. The first four values pushed by a function are required to be the values of the four registers `fp`, `sp`, `lr`, and `pc`. These four saved registers form the "full frame". In addition, the register `r11` is used a dedicated "frame pointer" (`fp`). The `fp` is set to point to the first word pushed in the current stack frame (contrast to `sp` which points to the last word pushed). The `fp` tells you where to access the full frame of the currently executing function. 

![stack diagram](../images/full_frame.png){: .zoom}

Above is a diagram taken from the lecture slides. Each stack frame begins by pushing the same 4 saved registers, this is the "full frame". The saved fp within a full frame points to the full frame of the caller. Following the sequence of fp links will lead back to the main function. This information is used by the debugger to produce a backtrace of the current call stack.  




