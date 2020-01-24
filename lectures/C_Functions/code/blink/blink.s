.equ DELAY, 0x3F0000

// configure GPIO 20 for output
ldr r0, FSEL2
mov r1, #1
str r1, [r0]

// set bit 20
mov r1, #(1<<20)

loop: 

ldr r0, SET0    // turn on
str r1, [r0] 

mov r0, #DELAY
mov r14, pc
b delay

ldr r0, CLR0    // turn off
str r1, [r0] 

mov r0, #DELAY >> 3
mov r14, pc
b delay

b loop

delay:
wait:
    subs r0, #1
    bne wait
    mov pc, r14


FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C
