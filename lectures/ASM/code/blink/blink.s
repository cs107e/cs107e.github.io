.equ DELAY, 0x3F0000

// configure GPIO 20 for output
ldr r0, FSEL2
mov r1, #1
str r1, [r0]

// set bit 20
mov r1, #(1<<20)

loop: 

// Set pin 20 high
ldr r0, SET0
str r1, [r0]
// wait
mov r2, #DELAY
wait0:
  subs r2, r2, #1
  bne wait0


// Set pin 20 low (clear)
ldr r0, CLR0
str r1, [r0]
// wait
mov r2, #DELAY
wait1:
  subs r2, r2, #1
  bne wait1

b loop

FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C

