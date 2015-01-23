.global _start
_start:

bl init

loop: 

    // set GPIO 20 high
    mov r0, #20
    bl set

    // delay
    mov r0, #0x3F0000
    bl delay

    // clr GPIO 20 low
    mov r0, #20
    bl clr

    // delay
    mov r0, #0x3F0000
    bl delay

b loop

// configure GPIO 20 for output
init:
    ldr r1, =0x20200008
    mov r0, #1
    str r0, [r1]
    bx lr

// set bit r0 in GPIO SET0 register
set:
    mov r1, #1
    lsl r0, r1, r0
    ldr r1, =0x2020001C // SET0
    str r0, [r1] 
    bx lr

// clr bit r0 in GPIO SET0 register
clr:
    mov r1, #1
    lsl r0, r1, r0
    ldr r1, =0x20200028 // CLR0
    str r0, [r1] 
    bx lr

// delay for r0 loop iterations
delay:
    subs r0, #1
    bne delay
    bx lr
    
