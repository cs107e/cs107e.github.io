// turn on LED connected to GPIO20

// configure GPIO 20 for output
//  FSEL2 = 0x20200008
mov r0, #0x20000000
orr r0, #0x00200000
orr r0, #0x00000008
mov r1, #1    // indicates OUTPUT
str r1, [r0]  // store 1 to address 0x20200008

// set GPI020 to 1
//  SET0 = 0x2020001c
mov r0, #0x20000000
orr r0, #0x00200000
orr r0, #0x0000001c
mov r1, #1    
lsl r1, #20   // 1<<20
str r1, [r0]  // store 1<<20 to address 0x2020001c

// loop forever
loop:
b loop

