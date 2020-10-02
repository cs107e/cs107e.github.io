.equ DELAY, 0x3F0000

ldr r0, FSEL2   // configure GPIO 20 for output
mov r1, #(1<<0)
str r1, [r0]

mov r1, #(1<<20)  // bit 20

loop:

ldr r0, SET0    // turn on GPIO 20
str r1, [r0]

mov r2, #DELAY
wait1:
    subs r2, #1
    bne wait1

ldr r0, CLR0    // turn off GPIO 20
str r1, [r0]

mov r2, #DELAY
wait2:
    subs r2, #1
    bne wait2

b loop


FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
CLR0:  .word 0x20200028
