---
permalink: /demos/
title: Demos
---

Here we have gathered links to various interactive demos and visualization tools that CS107E students have found useful in the past. Check them out!

## Raspberry Pi
- [Interactive Raspberry Pi GPIO Pinout](https://pinout.xyz/). Nice way to see alternate
  functions by clicking each pin. Also, click a Ground pin to see all the
  other Ground pins.

## ARM
- An
[demo of the ARM immediate value encoding](https://alisdair.mcdiarmid.org/arm-immediate-value-encoding/#play-with-it). A tribute to how much can be done with just an 8-bit value and barrel shifter.
<a name="visual"></a>
- [Download VisUAL simulator](https://salmanarif.bitbucket.io/visual/downloads.html). This tool emulates the ARM instruction set and allows you to step through an assembly program. Pat used this tool in lecture. A few notes about VisUAL:
    
    - VisUAL supports a [limited list of instructions](https://salmanarif.bitbucket.io/visual/supported_instructions.html).
    - The instructions must be specified in ARM syntax which [differs slightly](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0773a/chr1383143764305.html) from the GNU syntax we use in the course. For example, in ARM syntax, there are no colons after labels and the comment character is semi-colon.
    - In the VisUAL editor, position your cursor on an instruction and use `Ctrl+Space` to get a pop up window with detailed information and examples of that instruction.
    - Keep in mind that VisUAL is a simulator, not an actual Raspberry Pi! There are no GPIO pins or peripheral registers. It simulates executing the assembly instructions, but obviously nothing will happen when it tries to manipulate those special memory addresses.
    - For further information, see the [VisUAL user manual](https://salmanarif.bitbucket.io/visual/).

## C
- Use the Godbolt [Compiler Explorer](http://gcc.godbolt.org/) to enter C code and see the mapping to compiled assembly. Choose `C` language, compiler `ARM gcc 9.2.1(none)` and flags `-Og` to approximate the toolchain we use in the course.

- Type a C program into [Rextester](https://rextester.com/l/c_online_compiler_gcc ) and it will compile and execute in a virtual environment.
Handy for quickly running a C snippet to see what it does.

- Enter a C program into [C Tutor](http://pythontutor.com/c.html#mode=edit) and you can step through and visualize its execution, including showing the contents of the stack and heap memory.

- [cdecl.org](http://cdecl.org/) converts cryptic C declarations to and from English.
