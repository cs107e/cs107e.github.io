// configure GPIO 10 for input
ldr r0, FSEL1
mov r1, #0
str r1, [r0]

// configure GPIO 20 for output
ldr r0, FSEL2
mov r1, #1
str r1, [r0]

// bit 10
mov r2, #(1<<10)

// bit 20
mov r3, #(1<<20)

loop: 
	// read GPIO 10 
	ldr r0, LEV0
	ldr r1, [r0] 
	tst r1, r2
	beq on // when the button is pressed (goes LOW), turn on LED
        
        // set GPIO 20 low
    off:
        ldr r0, CLR0
        str r3, [r0]
        b loop

        // set GPIO 20 high
    on:
        ldr r0, SET0
        str r3, [r0]
        b loop

FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C
LEV0:  .word 0x20200034
LEV1:  .word 0x20200038
