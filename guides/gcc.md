---
layout: page
title: Guide to Bare Metal Programming with GCC
---

*Written by Pat Hanrahan*

Compiling programs that run bare metal is different than compiling
programs that run under an operating system and use
standard libraries. `gcc` is configured by default to
assume you are using an operating system and the standard libraries,
which is how most programs are developed. 
Therefore, in order to compile bare metal programs
we need to provide the appropriate options.

For example,

     % arm-none-eabi-gcc -ffreesstanding -nostdlib -nostartfiles -c blink.c

### Freestanding

The option `--freestanding` instructs `gcc` to compile
the program without an operating system or standard libraries.
The program does not *stand* on top of an operating system or library.
It is *on its own* and free to do whatever it wants.
Operating systems and libraries will not get in its way.
The opposite of freestanding is hosted.
In hosted mode, the program runs at the pleasure of the host operating system.

In freestanding mode only the following header files
are included: float.h, iso646.h, limits.h, stdarg.h, 
stdbool.h, stddef.h, stdint.h (C99 standard 4.6).
These headers define the types appropriate for the machine
being used, as well as useful constants such as
the minimum and maximum values for different types.

The `-ffreestanding` option directs the compiler to not assume that
standard functions have their usual definition. 

Freestanding mode does not associate any special semantics to the function main.
Normally, main is predeclared as follows:

    int main(int argv, char *argv, char **env)

and the compiler will issue warnings if you define main differently.
In freestanding mode, main can have any type signature.
For example, in freestanding mode `main` may be called
without any input arguments, and will return anything.
In freestanding mode, you can define main as

    void main(void)
    
Freestanding mode does not change the linking behaviour.
For that, you need to use the `-nostdlib` option, 
or possibly `-nostartfiles` or `-nodefaultlibs`.

`gcc` normally performs many optimizations.
For example, in a hosted environment,
`gcc` can assume that there is a complete library available 
that meets the specification of the language standard being used 
(the default for C is gnu90, the GNU extension to the ISO C90 standard).
That means, for example, it can transform `printf("hi\n")` into `puts("hi")`
because it *knows* from the definition of the
standard IO library that these two functions are equivalent.
When `-ffreestanding` is used, `gcc` does not assume
that you are using a standard library environment.
In freestanding mode, you could define your own `puts`
function and your version of `puts` 
could act completely differently than
the standard puts function.

In a freestanding environment 
(in which C program execution may take place 
without any benefit of an operating system),
the name and type of the function called 
at program startup are implementation-defined.
Any library facilities available to a freestanding program,
other than the minimal set required by clause 4, are implementation-defined.

### Standard libraries and include files

`gcc` accepts several flags that control the use of standard
libraries and include files.

Options include

    -nostdinc 
   
This option sets the include path so that the 
standard C include directories cannot be found.
If you try to include a standard header file,
the compiler will issue an error.

    -nodefaultlibs

Instructs the compiler to not use the standard system libraries when linking. 
Only the libraries you specify will be passed to the linker.
Examples of standard libraries are `libc` and `libm`.

    -nostdlib

Instructs the compiler to not link to the standard system library,
as well as to not link to the standard system startup library. 
No startup files and only the libraries you specify are passed to the linker.

If the program is being compiled in hosted mode,
the compiler may generate calls to `memcmp`, `memset`, `memcpy` and `memmove`.
These entries are usually resolved by entries in `libc`.
These entry points should be supplied through some other mechanism 
when this `-nostdlib` is specified.

If you use `-nostdlib` and want to include libraries,
you need to specify linker paths and libraries explicitly with -L and -l.

One of the standard libraries that is not linked in when
using `-nostdlib` and `-nodefaultlibs`
is `libgcc.a`, a runtime library implementing 
functions that are needed to compile C.
For example, the ARM processor does not include a division instruction. 
`libgcc.a` includes functions to perform division.
If you do not provide an implementation of these functions,
the program will not run.

If you use `-nostdlib`, 
then you need to explicitly link with `libgcc.a` (`-lgcc`)

### Start files

Normally, when a program begins to run,
a start function is called.
This function sets up the machine to run the program.
The most common task performed by start functions are
to initialize default values for any variables in your program.

The option

    - nostartfiles

instructs the compiler to not call the 
standard system startup functions and to not link to the
libraries containing those functions.

If you don't link to a start function,
program variables may not be properly initialized.
You may need to provide your own start function
when running in freestanding mode.

