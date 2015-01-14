/* configure GPIO 20 for output */
ldr r0, =0x20200008 
mov r1, #1
str r1, [r0]

// set bit 10
mov r2, #(1<<10)

// set bit 20
mov r3, #(1<<20)

loop: 

// read GPIO 10 
ldr r0, =0x20200034
ldr r1, [r0] 
tst r1, r2
bne lo

// set GPIO 20 high
hi:
ldr r0, =0x2020001C
str r3, [r0] 
b loop

// set GPIO 20 low
lo:
ldr r0, =0x20200028
str r3, [r0] 
b loop


