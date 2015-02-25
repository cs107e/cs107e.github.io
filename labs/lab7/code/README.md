Application that shows internal processor state (sp, fp, GPIO registers, etc.)
in a while loop. It handles interrupts. Each execution increments a counter by 0x1,
while each interrupt (falling edge on GPIO 23) increments the counter by 0x1000.
Therefore, plugging in a keyboard and typing a basic key will increase the counter
by 0x21000: 33 increments of 0x1000 (11 for each scan code, 3 scan codes). 
If you do not enable interrupts in notmain(), you can see how the event detection
register (GPEDS0) contains a pending event.

start.s has _start, where the program begins (0x8000). This sets up the stacks
for supervisor mode (0x7000) and interrupt mode (0x8000), then jumps to _cstart.

cstart.c has _cstart, which does two things. First, it zeroes the BSS. Second,
it copies the interrupt table in interrupts_asm.s to 0x0. It then invokes
notmain.

interrupts_asm.s has the interrupt table and its embedded constants as well as
the assembly routines that correctly save processor state before jumping to C code
handlers.

handlers.c contains the C handlers invoked from interrupts_asm.s.

interrupt_display.c contains notmain and the application.

