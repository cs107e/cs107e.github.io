---
permalink: /demos/
title: Demos and Tools
---

Here we have gathered links to demos and visualization tools that CS107E students have found useful. Check them out!

## RISC-V
<a name="ripes"></a>
- __Ripes simulator__, simulator/visualizer/assembly editor for RISC-V
    - Run it in your brower: <https://ripes.me/>
    - Or download original version as desktop application (Qt)
    - Source code on github <https://github.com/mortbopet/Ripes>, pre-built binaries are available <https://github.com/mortbopet/Ripes/releases>
- __Rars simulator__, adapted from MARS, popular for teaching MIPS
    - Desktop application (java) <https://github.com/TheThirdOne/rars>
- __Cornell simulator__
    - Created for a course at Cornell, somewhat simplified. <https://www.cs.cornell.edu/courses/cs3410/2019sp/riscv/interpreter/>
- __Instruction encoder/decoder__ from Luplab UC Davis
    -  <https://luplab.gitlab.io/rvcodecjs/#q=fffff06f&abi=true&isa=RV64I>

## C language
- Matt Godbolt's __Compiler Explorer__ is an awesome tool for seeing how the compiler translates from C to assembly
    - <https://gcc.godbolt.org/> Configure the settings for the `C` language, compiler `RISC-V(64-bits) gcc 12.20` and flags `-Og -ffreestanding` to approximate the toolchain we are using. Here is a preconfigured link <https://gcc.godbolt.org/z/Eod3jo3bb>.
- Use __Rextester__ to compile and execute C program in a virtual environment.  <https://rextester.com/l/c_online_compiler_gcc>
   - Very handy for quickly running a C snippet to see what it does.
- __C Tutor__ is a nifty tool where you can step through a C program and visualize its execution, including showing the contents of the stack and heap memory. <http://pythontutor.com/c.html#mode=edit>
    - (also available for python, javascript, java). From Philip Guo UC San Diego
- The famed __cdecl.org__ <http://cdecl.org/> is your go-to when you need to convert a cryptic C declaration to English.
