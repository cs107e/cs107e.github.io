---
title: Binary Utilities (binutils)
toc: true
---

This document describes binutils (binary utilities). These are utility programs used for compiling,
inspecting, and transforming binaries (machine code files). Some of them you've probably
heard of or used before. Other, more specialized ones, might be new to you. Learning these
tools and what they can do will greatly improve your productivity as a programmer. They'll
let you quickly and easily figure out what is wrong and why with a large number of otherwise
very difficult problems.

This guide refers to each tool with its short name (e.g., `ld`, `as`). Typically, on a UNIX/Linux/Darwin
machine, these names refer to the native tools, tools for that computer. For example, `gcc` on
a myth machine is a version of the Gnu C Compiler for Linux on an x86 processor. Since we're
using a laptop to cross-compile for the Raspberry Pi, you want to use the binary utilities
that are designed to run on your machine but compile for the Raspberry Pi. These tools are
prefixed with `arm-none-eabi`. So you want to use `arm-none-eabi-ld` not `ld`.


### Core utilities

This section discusses the core utilities that you use very often: `as`, `ld`, `nm`, `size`, `strings`,
`strip`, `objcopy`, `objdump`, and `ar`.

### `as`

This is the assembler. It takes assembly code (human readable text of machine code) and turns it
into a binary that a processor can execute. When you invoke `gcc` and tell it to generate
a binary (e.g., an object file), gcc first compiles the C code to assembly then invokes the
assembler to generate machine code. The output of `as` is a file format called ELF, or
Executable and Linkable Format. Almost all executables you run in Linux, for example, are ELF.
ELF includes not only machine code (the E part) but also *symbols*, names that provide
information on where different variables and functions reside (see `nm` and `objdump` below).
When you compile a library,
for example, the resulting ELF file has not only the library's code, but also a set of symbols
that say where the code for each library function starts. That way, when you compile a program
that invokes the library function, binutils can find where it is and link the two files together
correctly (see `ld` below).



The most common use of `as` is as follows:

    $ as code.s -o code.o

This command instructs the assembler to read assembly code from the input file `code.s` and to write the machine code to the output file `code.o`. If you do not include `-o code.o` then it will default to a output file named to
`a.out`. 

Just as the input to `as` might not be a executable program, the output of `as` might
not be executable. For example, a library is a set of functions that other programs can
call, but does not have any entry point (`main()` function). It can also be that the
compiled assembly code references functions that are in another binary file. For example,
your program that calls a library, when compiled into machine code, does not have the
actual instructions of those library calls. The tool that takes multiple binary files
and links them together is `ld`, described below.

### `ld`

The linker. `ld` takes object files, libraries, and other binary files as input and links
them together into another binary file. For example, when you have a library written in
multiple source files, you typically compile each source file independently into an ELF
object file, then link all of those files together into a larger ELF object file for the
library. When you compile a program that uses the library, you compile your program code
into one or more object files, then link those against the library, producing a final
executable.

The linker operates by resolving *symbols*. The name of each of your functions and global variables is a symbol. Symbols come in two forms: a definition that associates a name with its data (e.g. initializing a global variable or the body of a function) and a declaration/use of that name.  There must be exactly one definition for a symbol, but there can be multiple declarations/use of the symbol. For example, when you define a variable `int a = 17;`
in global scope, this creates a symbol for `a`, which establishes at what memory address `a`
resides and sets its initial value to 17. Any use of the global `a` must be resolved to refer to this one copy of the variable. Similarly with a function named `f()`. There will be one definition of `f()` (perhaps in a library). Every other place where you call the function `f()` it generates an unresolved reference. It is the job of the linker to resolve all such references to use the one shared copy.  When the linker links your program,
it sees that `f` is an unresolved symbol, sees where `f` is defined in the library,
then fixes the binary code in your program so that when it calls `f` it jumps to
where `f` exists.

The basic use for `ld` is as follows:

    $ ld code1.o code1.o -o code.bin

This takes two object files, links them together, and outputs the result as `code.bin`.

When you invoke GCC without the `-c` option, it typically runs `ld` as its last step. For
example,

    $ gcc -o test test.c

is mostly equivalent (I'm leaving out a lot of options that are added by default) to:

    $ gcc -S test.S test.c  # Generate assembly
    $ as -o test.o test.S   # Generate object file
    $ ld -o test test.o 

The most commonly used options for `ld` are `-l` and `-L`. The `-l` option tells `ld`
to link a library, which is basically just an object file. But libraries are typically
stable code that you don't update often and are used by many programs, so they live
in separate directories for that purpose (e.g, `/usr/lib`). The `-L` option tells `ld`
what directories to look for libraries in. So, for example,

    $ ld -o test test.o -lgcc -L/usr/lib

tells `ld` to link a file name `libgcc.a` (or `libgcc.so`, but we will not be using
shared objects in this class, so don't worry about them) which it should search for
not only in the local directory but also `/usr/lib`. You can pass multiple `-l` and
`-L` options. For example, `gcc` will typically automatically pass `-lgcc` and `-L/usr/lib`
options when it invokes `ld`, which are in addition to any other library loads
or library search path entries you add.

### `nm`

Displays the symbol table of a binary file. It takes one or more file names as parameters
and outputs their symbol tables. The output looks like this:

    00010074 B __bss_end__
    00010070 B __bss_start
    00010070 B __bss_start__
    00010070 T __data_start
    00010074 B __end__
    00010074 B _bss_end__
    00010070 T _edata
    00010074 B _end
    00080000 N _stack
             U _start
    00010070 B a
    00008040 T f
    00008000 T main

The first column states the address of the symbol. For a function, this is the first instruction
to jump to when you call the function. For a variable, this is its location in memory. The
second column states what kind of symbol it is. The types are:

  1. `T`: Text. This is a symbol for executable code (e.g., a function).
  1. `D`: Data. This is a symbol for a variable that has an initializer (e.g., `int a = 5;`).
  1. `B`: Data. This is a symbol for a variable that doesn't have an initializer (e.g., `int a;`).
  1. `U`: Undefined. This a symbol the objects needs to be linked. It's defined elsewhere, and
     hopefully the linker will resolve it later.
  1. `N`: Symbol used for debugging.
  1. `A`: Absolute. Later linking will not change this symbol.

So in the above example, the object file defines a function `f`, a function `main`, and a variable `a`.
The other symbols (generally, symbols that start with `_`) are generated by the compiler for
bookkeeping and linking.

### `size`

Lists the size of sections (and total size) of object files. Can be invoked on multiple 
files at once simply by listing the desired files, 

    $ size test.o test2.o

which produces output like this:

     text	     data     bss      dec     hex	  filename
       80	       24      32      136 		88 	  test.o
       72	        0       0       72		48	  test2.o

Under `text` you see the size of the actual machine code that makes up your program. Similarly to 
the symbol types listed under `nm` above, the `data` section is the size of storing your initialized 
global variables, and `bss` is the size of storing your uninitialized global variables. The `dec` and 
`hex` numbers indicate the total size (sum of text, data, & bss) in decimal and hexadecimal, 
respectively.

The options for the size command are mostly to change the format of the output. For example, to see 
the section sizes in hexadecimal, use the `-x` option (`size -x test.o`). You can also specify the 
file type if it is not automatically recognized. For example if you want to know the size of a raw 
binary file, you could use 

    $ size --target=binary test.bin

### `strings`

Prints text strings embedded in the input file. `strings` is useful 
for searching binary files, which are not readable using a text editor. For example, if you 
wanted to search an object file for a particular string, you could call strings and pipe the result to 
grep, like this:

    $ strings test.o | grep "my string"

By default, `strings` looks for strings of at least 4
printable characters (followed by a NUL character indicating the end of a string).
To set a minimum string length other than 4, use the `-n` option. For example,

    $ strings -n 6 test.o

looks for strings of at least 6 characters.

### `strip`

Removes the symbol table from an object file. The symbol table has information about each symbol by name, including size, type, and address, 
see `ld` above. By stripping out symbol tables and debug information, `strip` decreases the size of 
object files.

`strip` modifies the input file rather than creating a new, stripped output file. Compare the file 
before and after you strip it using `nm` (above). You will find that the symbol information has been removed. All of the code/data for the symbols remains in the binary, but there is no longer a "legend" that identifies which symbol is where.

### `objcopy`

Transforms binary objects between different formats. For example, you can use objcopy to transform
an ELF executable (which has symbols and all kinds of other information) into a simple binary. The principal
options to `objcopy` are `-O` and `-I`, which specify the output and input formats. Example formats are
elf32-bigarm, ihex, and binary. Sometimes `objcopy` can tell what the format is and so doesn't need to be
told explicitly (e.g., ELF). In its most basic use, `objcopy` just makes a copy of the file. For example,

    $ objcopy main main2

creates a simple of main in main2. In contrast,

    $ objcopy main -O binary main.bin 

takes main as input (an ELF file), transforms it into a raw binary file and outputs that raw binary as main.bin.

### `objdump`

Displays information about object files. To use `objdump`, you must specify at least one of the many 
options, which indicate what type of information you would like to view. There are many different options, 
check out `man objdump` to see what it can show.

A very useful option is the `-d` option (`-d` is for disassemble), which allows you to view the assembly instructions 
associated with the executable part of the binary file:

    $ objdump -d test.o

### `ar`

Allows you to create, modify, and extract archives. Archives are single files holding 
collections of other (usually binary) files, similar to a zip or tar file. Archives are named with a `.a` extension 
and are usually used to hold libraries. The linker (see `ld` above) is often used to link to functions 
in these archive library files.

`ar` has options for you to create an archive, add or remove files from an existing archive, and 
extract files from an archive. To create an archive from object files, use the `cr` or 
`crs` options, followed by your object files. The `c` option means it will not warn you that it 
needs to create the library (since that is 
what you are trying to do). The `r` option says to insert the new files (or replace existing ones).
If you specify the `s` option, the archive maintains an
index to all symbols defined in files in the archive to allow for quicker linking to the library
functions.

For example,

    $ ar cr libtest.a test.o test2.o

makes the library archive file libtest.a containing test.o and test2.o. Then you can link to this 
library by specifying `-ltest` in the `ld` command (see `ld` above).

For more options on how to modify archives, see `man ar` or `ar --help`.

<!-- ## Other utilities -->

