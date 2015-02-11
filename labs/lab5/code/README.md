## Framebuffer and GPIO/PS2 source code

1. `Makefile`:  has rules for compiling .bin and .elf files. the DEFAULT rule 
   controls which .bin will be built automatically by `make` and will be
   installed with `make install`. If you are using Linux you will need to
   modify the `make install` command to use `/dev/ttyUSB`. The Makefile
   automatically compiles `start.o`, `cstart.o`, `led.o` and `helpers.o`
   and links them with your source file.

1. `start.s`: Assembly file that containts `_start` and `hang`. `_start`
   sets the stack pointer and calls `_cstart`. 

1. `cstart.c`: C file that contains `_cstart`. This zeroes out the BSS and
   then invokes `notmain` in your program. If `notmain` returns it turns
   on the green activity LED.

1. `led.c` and `led.h`: Contain functions for toggling the on-board
    activity LED (the green one next to the red power LED).

1. `helpers.s`: a few useful helper assembly routines.

1. `fb.c`: source file for basic framebuffer implementation. You will
    fill in this file to have the functions from `fb.h`.

1. `lines-1d.c`: source file for application that draws lines using a 1D
    framebuffer array.

1. `lines-2d.c`: source file for application that draws lines using a 2D
    framebuffer array.

1. `ps2-clock.c`: source file for blinking LED on PS2 clock line.

1. `ps2-data.c`: source file for blinking LED on PS2 data line.

1. `reference.img`: a binary image that provides a simple console
    which you can type into. Use this to check your hardware.

