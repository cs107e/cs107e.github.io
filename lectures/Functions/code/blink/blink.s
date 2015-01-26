.global _start

_start:

bl init

loop: 

    mov r0, #20
    bl set

    mov r0, #0x3F0000
    bl delay

    mov r0, #20
    bl clr

    mov r0, #0x0F0000
    bl delay

b loop

// delay
delay:
   wait:
      subs r0, #1
      bne wait
   bx lr

// configure GPIO 20 for output
init:
    ldr r0, =0x20200008
    mov r1, #1
    str r1, [r0]
    bx lr

// set GPIO 20 high
set:
   mov r1, #1
   lsl r1, r1, r0
   ldr r0, =0x2020001C // SET0
   str r1, [r0] 
   bx lr

// set GPIO 20 low
clr:
   mov r1, #1
   lsl r1, r1, r0
   ldr r0, =0x20200028 // CLR0
   str r1, [r0] 
   bx lr

