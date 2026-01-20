---
title: "Guide: Bare-Metal Programming using gcc"
attribution: Written by Pat Hanrahan and Julie Zelenski
---


This guide gives a brief overview of what is unique about compiling C programs to execute in a bare-metal environment.

## Hosted versus non-hosted environments
A typical program is compiled for a _hosted_ system. In hosted mode, the program runs at the pleasure of the host operating system. The executing program has access to
the standard libraries and facilities provided by the operating system layer (filesystem, networking, hardware peripherals, and more) but must coordinate its use of these resources according to terms set by its OS overlord.

In contrast, a bare-metal program is non-hosted; it does not stand on top of an
operating system or any libraries; it runs entirely on its own and cannot count on any facilities other than what it provides for itself. The executing program has unfettered access to all resources, as there are no other processes running to interfere, supervise, or restrict its actions.

In compiler-speak, the non-hosted
environment is referred to as __freestanding__ or __standalone__. In this course, all
of our programs are compiled for the bare-metal freestanding environment.

By default, `gcc` compiles assuming a hosted environment.
To build for bare-metal, we must set the appropriate compiler and
linker options.

## Compiler option `-ffreestanding`
This `gcc` option directs the compiler to limit this program to only those features available in the freestanding environment.

```console
$ riscv64-unknown-elf-gcc -ffreestanding -c blink.c
```

When compiling freestanding, the only available standard header files are:
`<float.h>`, `<iso646.h>`, `<limits.h>`, `<stdarg.h>`, 
`<stdbool.h>`, `<stddef.h>`, and `<stdint.h>` (C99 standard 4.6).
These headers define the types specific to the target architecture and constants for the minimum and maximum values per type.
All other standard header files (`<stdio.h>`, `<string.h>` and so on) are __not available__;
these headers cannot be included or used.

When a hosted program starts, `main` is the function where the program execution begins. This means a hosted program must define a function called `main` with the required signature:

    int main(int argv, char *argv[])   // main in hosted env (args passed externally)

It is an error in hosted mode if the `main` function is missing or signature does not match above.

The `-ffreestanding` option removes the special semantics for the `main` function. A bare-metal program must provide its own start sequence (see below) and thereby decides the name and signature of the function. In our bare-metal build process, we adopt `main`
as the name for the main program function. The signature we will use is simply:

    void main(void)                    // main in bare-metal env (no args)

When compiling freestanding, the compiler will disregard all assumptions about the existence and operation of functions from the standard library.  In the default hosted environment, the standard functions are known to exist and are a standards-compliant implementation. The compiler can leverage that knowledge to make various optimizations and transformations.  For example, `gcc` can transform `printf("hi\n")` into `puts("hi")`
because it _knows_ the two functions behave equivalently in this case and prefers the lighter weight operation. Or consider the loop `for (int i = 0; i < strlen(s); i++)` where `s` is a constant string. In a compliant implementation, `gcc` knows that every call to `strlen` on a constant string is guaranteed to return the same value, so it can hoist the `strlen` call outside the loop and compute just once rather than repeat call on every loop iteration.  These are clever optimizations, but the assumptions they depend on are not guaranteed to hold in the bare-metal environment.  By using `-ffreestanding`, you are teling `gcc` to not assume a standard library environment and to not apply such substitutions or rearrangements.



## Linker option `-nostdlib`
The linker option `-nostdlib` is used to link a program intended to run freestanding. `-nostdlib` implies
the individual options `-nodefaultlibs` and `-nostartfiles`. Below we describe
the two options separately, but common use case is `-nostdlib` to get both options as a package deal.

### `-nodefaultlibs`
When linking a hosted program, standard system libraries such as `libc` are 
linked by default, giving the program access to all standard 
functions (`printf`, `strlen` and friends).  The linker option `-nodefaultlibs`
disables linking with those default libraries; the only libraries linked are
exactly those that you explicitly name to the linker using the `-l` flag.

`libgcc.a` is a small library of compiler support routines.
For example, a specific RISC-V processor may not have hardware support for floating point.  Linking with `libgcc.a` substitutes functions that emulate the floating point operations in software.
  A program that attempts to use software floating point and is linked `-nodefaultlibs` will fail to link. The linker errors will be something
akin to

```console
riscv64-unknown-elf-ld: main.c:9: undefined reference to `__floatsidf'
riscv64-unknown-elf-ld: main.c:10: undefined reference to `__adddf3'
riscv64-unknown-elf-ld: main.c:10: undefined reference to `__divdf3'
```
Linking with `libgcc.a` (`-lgcc`) will provide the missing routines and resolve these references.

Note that `libgcc` does __not__ supply `memcpy` and related functions. 
Buried deep in <https://gcc.gnu.org/onlinedocs/gcc/Standards.html>, there is a small
callout that notes this:

<q>Most of the compiler support routines used by GCC are present in libgcc,
but there are a few exceptions. GCC requires the freestanding environment
 provide `memcpy`, `memmove`, `memset` and `memcmp`.</q>

These low-level routines copy, initialize, or compare raw bytes of a chunk of memory. Even though your code does not make explicit calls to these functions, you still may need to supply definitions due to
gcc inserting implicit calls as needed to initialize an array or pass a large struct in or out of a function. A freestanding program must supply its own implementations of these functions since it does not have access to the standard library.


### `-nostartfiles`
When a program is loaded and begins executing, the very first code to run is the _start sequence_.
The purpose of the start sequence is to configure hardware settings, initialize the program memory segments (stack, global data, heap), and only then call the `main` function.


The standard start sequence is linked by default and assumes a hosted environment and standard library, thus it cannot be used for a bare-metal program. The option `-nostartfiles`
instructs the linker to not link the standard sequence. If you don't link to a start function, program state will not be properly initialized. A bare-metal program must provide an alternate start sequence that is customized for the target system.

Review files [start.s](/src#start) and [cstart.c](/src#cstart) for the custom code we use as our start sequence.

### Linker map `-T memmap.ld`
A linker map defines how the program sections are laid out in memory (the relative positions of instructions, data, bss, stack, and heap) when the program is executing. By default, the linker uses the standard linker map, which is perfect for a hosted system, but totally wrong for bare-metal.  A bare-metal build needs to override that default and provide its own custom linker map using the linker option `-T memmap.ld`. In addition to laying out the data sections, the linker map also ensures the start sequence is the first code to execute.

Review file [memmap.ld](/src#memmap) for the custom linker map we use.


