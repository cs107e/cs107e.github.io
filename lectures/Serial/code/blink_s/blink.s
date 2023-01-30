.equ DELAY, 0x3F0000

ldr r0, FSEL2   // configure GPIO 20 for output
mov r1, #(1<<0)
str r1, [r0]

mov r1, #(1<<20)  // bit 20

loop:

ldr r0, SET0    // turn on GPIO 20
str r1, [r0]

mov r0, #DELAY
bl pause

ldr r0, CLR0    // turn off GPIO 20
str r1, [r0]

mov r0, #DELAY
bl pause

b loop

pause:
    subs r0, #1
    bne pause
    bx lr


FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
CLR0:  .word 0x20200028
