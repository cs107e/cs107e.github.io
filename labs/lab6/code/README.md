## Framebuffer and GPIO/PS2 source code

1. `Makefile`:  has rules for compiling .bin and .elf files. the DEFAULT rule 
   controls which .bin will be built automatically by `make` and will be
   installed with `make install`. If you are using Linux you will need to
   modify the `make install` command to use `/dev/ttyUSB`. The Makefile
   automatically compiles `start.o`, `cstart.o`, `led.o`, `gpio.o`,
   and `helpers.o` and links them with your source file.

1. `start.s`: Assembly file that containts `_start` and `hang`. `_start`
   sets the stack pointer and calls `_cstart`. 

1. `cstart.c`: C file that contains `_cstart`. This zeroes out the BSS and
   then invokes `notmain` in your program. If `notmain` returns it turns
   on the green activity LED.

1. `led.c` and `led.h`: Contain functions for toggling the on-board
    activity LED (the green one next to the red power LED).

1. `helpers.s`: a few useful helper assembly routines.

1. `gpio.c` and `gpio.h`: source files for basic GPIO functionality.

1. `system.c` and `system.h`: source files for low-level operations, such
    as cache control and interrupt enabling/disabling.

1. `timer.c` and `timer.h`: a low-level timer. This might conflict with
    a `uwait` implementation, so be careful.



