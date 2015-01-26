.global _start
_start:

#define flash(pin) mov r0, #(1<<pin); bl toggle

bl init

loop: 

    flash(20)
    flash(21)
    flash(22)

    flash(23)

    flash(22)
    flash(21)

b loop

// toggle(pin)
toggle:
    mov r5, lr // backup because we are calling other functions
    mov r4, r0 // backup pin
    bl set
    mov r0, #0x3F0000
    bl delay
    mov r0, r4
    bl clr
    bx r5

// set bit r0 in GPIO SET0 register
set:
    ldr r1, =0x2020001C // SET0
    str r0, [r1] 
    bx lr

// clr bit r0 in GPIO SET0 register
clr:
    ldr r1, =0x20200028 // CLR0
    str r0, [r1] 
    bx lr

// delay for r0 loop iterations
delay:
    subs r0, #1
    bne delay
    bx lr

// configure GPIO 20-23 for output
init:
    mov r0, #1
    orr r0, r0, #(1<<3)
    orr r0, r0, #(1<<6)
    orr r0, r0, #(1<<9)
    ldr r1, =0x20200008
    str r0, [r1]
    bx lr

    
