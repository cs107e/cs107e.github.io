// File: c_blink.c
// ---------------
// C translation of blink.s asm program

void main(void) {
    unsigned int *PB_CFG0 = (unsigned int *)0x2000030;
    unsigned int *PB_DATA = (unsigned int *)0x2000040;

    *PB_CFG0 = 1;   // config as output

    int state = 1;
    while (1) {
        state = state ^ 1;    // toggle state on/off
        *PB_DATA = state;     // set LED to state
        int count = 0x3f00000;
        while (--count != 0)  //  delay loop
            ;
    }
}
