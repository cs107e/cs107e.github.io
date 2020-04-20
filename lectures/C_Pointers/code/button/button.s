ldr r0, FSEL1  // configure GPIO 10 for input
mov r1, #0
str r1, [r0]

ldr r0, FSEL2 // configure GPIO 20 for output
mov r1, #1
str r1, [r0]

mov r2, #(1<<10)   // bit 10
mov r3, #(1<<20)  // bit 20

loop: 
    ldr r0, LEV0
    ldr r1, [r0]  // read GPIO 10 
    tst r1, r2
    beq on        // if button down (reads low), turn on LED, else turn off
    
    off:         // set GPIO 20 low
        ldr r0, CLR0
        str r3, [r0]
        b loop

    on:          // set GPIO 20 high
        ldr r0, SET0
        str r3, [r0]
        b loop

FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
 SET0: .word 0x2020001C
 CLR0: .word 0x20200028
 LEV0: .word 0x20200034
