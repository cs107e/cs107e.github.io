// turn on LED connected to GPIO20

// configure GPIO 20 for output
//
// GPIO 20 function select is in the 2nd function select register
//

mov r0, #0x20      // #0x00000020
lsl r1, r0, #24    // #0x20000000
lsl r2, r0, #16    // #0x00200000
orr r0, r1, r2     // #0x20200000
orr r0, r0, #0x08  // #0x20200008
mov r1, #1         // indicates OUTPUT
str r1, [r0]       // store 1 to address 0x20200008


// set GPI020 to 1
//
// GPIO 20 is in the 1st SET register
//
// GPIO 20's output value is controlled by bit 20
//
// SET0 = 0x2020001c

mov r0, #0x20
lsl r1, r0, #24    // #0x20000000
lsl r2, r0, #16    // #0x00200000
orr r0, r1, r2     // #0x20200000
orr r0, r0, #0x1c  // #0x2020001c
mov r1, #1    
lsl r1, #20        // #0x00100000
str r1, [r0]       // store 1<<20 to address 0x2020001c

// branch forever
loop:
b loop // go to loop

