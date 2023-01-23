    ldr r0, FSEL1   // config GPIO 10 as input
    mov r1, #0
    str r1, [r0]

    ldr r0, FSEL2   // config GPIO 20 as output
    mov r1, #1
    str r1, [r0]

    mov r2, #(1<<10)    // bit 10
    mov r3, #(1<<20)    // bit 20

    ldr r0, SET0
    str r3, [r0]    // set GPIO 20 (LED on)

wait:
    ldr r0, LEV0
    ldr r1, [r0]    // read LEV0
    tst r1, r2      // test bit 10
    bne wait        // if button not pressed, keep waiting

    ldr r0, CLR0
    str r3, [r0]    // clear GPIO 20 (LED off)

hang: b hang

FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
 SET0: .word 0x2020001C
 CLR0: .word 0x20200028
 LEV0: .word 0x20200034
