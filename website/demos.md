---
permalink: /demos/
title: Demos
---

Here we have gathered links to various interactive demos and visualization tools that CS107E students have found useful in the past. Check them out!

### Raspberry Pi
- [Interactive Raspberry Pi GPIO Pinout](https://pinout.xyz/). Nice way to see alternate
  functions by clicking each pin. Also, click a Ground pin to see all the
  other Ground pins.

### ARM
- An
[interactive demo of the ARM immediate value encoding](https://alisdair.mcdiarmid.org/arm-immediate-value-encoding/#play-with-it). A tribute to how much can be done with just an 8-bit value and barrel shifter.
<a name="visual"></a>
- [Download VisUAL](https://salmanarif.bitbucket.io/visual/downloads.html) and play with it. This simulator emulates the ARM instruction set and allows you to step through an assembly program. Pat used this tool in lecture. A few notes on using this tool:
    
    - VisUAL has a limited [list of supported instructions](https://salmanarif.bitbucket.io/visual/supported_instructions.html).
    - The instructions must be specified in ARM syntax which [differs slightly](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0773a/chr1383143764305.html) from the GNU syntax we use in the course. For example, in ARM syntax, there are no colons after labels and the comment character is semi-colon.
    - In the VisUAL editor, position your cursor on an instruction and use `Ctrl+Space` to get a pop up window with detailed information and examples of that instruction.
    - Another gotcha is that VisUAL is not actually a Raspberry Pi! There are no GPIO pins or peripheral registers. You can simulate the code, but obviously nothing will happen when it tries to manipulate GPIOs.
    - For further information, see the [VisUAL user manual](https://salmanarif.bitbucket.io/visual/).

### C
- Use the Godbolt [Compiler Explorer](http://gcc.godbolt.org/) to enter C code and see the mapping to compiled assembly. Choose `C` language, compiler `ARM gcc 5.4.1(none)` and flags `-O` to approximate the toolchain we use in the course.

- Type a C program into [Repl](https://repl.it/languages/c) and it will compile and execute in a virtual environment.
Handy for quickly running a C snippet to see what it does.

- Enter a C program into [C Tutor](http://pythontutor.com/c.html#mode=edit) and you can step through and visualize its execution, including showing the contents of the stack and heap memory.

- [cdecl.org](http://cdecl.org/) converts "C gibberish" declarations to and from English.
