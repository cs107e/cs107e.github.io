---
title: 'Stack frames'
attribution: Written by Julie Zelenski
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

+ At program start, we reserve a fixed-sized region to hold the stack and set a
pointer (the stack pointer) to the start of that region.
+ When entering a new function, the memory the function needs is allocated in a contingous single chunk by adjusting
the stack pointer. This memory is referred to as the function's _stack frame_.
+ When exiting the function, the memory used for its stack frame is recycled by restoring the stack pointer to its position when the function was entered.

On the Mango Pi, the stack grows downward.  When making a function call,
the data for the callee function is stored at a lower address than the caller's data.
It looks like this:

![stack picture](../images/stack.png)

This organization is so effective that compilers explicitly
support management of the stack pointer and stack frames,
architecture manuals provide the rules for how to do so, and an ISA may even include designated instructions for stack push and pop.

### Managing a stack on RISC-V architecture

Register `x2` (synonym `sp`) is designated for use as the stack pointer.
`sp` points to the location of the last value added to the stack. This value is
referred to as the "top" of the stack. (yes, it is confusing that the top is stored at the lowest address!)

The stack operates as a _descending_
stack. This stack pointer is initialized to its highest
address. Adding a new value to the stack moves the `sp` downward (to a lower address).
Removing a value from the stack is accomplished by incrementing the `sp`.

When entering a function, the `sp` is adjusted downward to reserve space for the new stack frame.
```
addi sp,sp,-16
```

Registers that need to be saved store the value into the stack frame using a sp-relative offset.
```
sd ra,8(sp)
sd fp,0(sp)
```
When exiting the function, load the saved contents from the stack frame to restore the register's saved value.
```
ld ra,8(sp)
ld fp,0(sp)
```

And finally adjust the stack pointer upward to "remove" the stack frame:
```
addi sp,sp,16
```

The data being stored on the stack might be caller-owned registers or intermediate results. Large local variables (e.g. arrays and structs) that are too large to store in a register are also stored on the stack. The adjustment to the `sp` will be the total space needed for all the saved registers and local variables.

### Use of frame pointer
The executing function can access data on the stack using relative offsets from the `sp` register.  The compiler has latitude in what it stores on the stack; there is no guarantee about what values are stored in each frame and how values are laid out. This makes it challenging if trying to ascertain where each stack frame starts and stops.

Applying the compiler flag `f-no-omit-frame-pointer` tells the compiler to use a frame pointer when managing the stack. This enforces a particular order on the stack frame layout. The first two values pushed by a function will always be the saved values of the two registers `ra` and `fp`.  In addition, the register `s0` is used a dedicated "frame pointer" (`fp`).  Whereas the `sp` points to the last word of the _current_ frame, `fp` is set to point to the last word of the _previous_ frame. Thus the range from `fp` to `sp` bookends the extent of the current stack frame.  The `fp` tells you where to access the start of the frame of the currently executing function.

![stack diagram](../images/frame_pointers.png){: .zoom}

Above is a diagram taken from the lecture slides. The instructions in the function prolog save the values of the `ra` and `fp` registers by pushing them onto the stack. These two saved registers are always the first 16 bytes of the current stack frame. The `saved fp` is saving the value of `fp` from its caller. Following the sequence of fp links will lead back to the main function. This information is used by the debugger to produce a backtrace of the current call stack.

