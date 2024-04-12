void main(void) {
    unsigned int *PB_CFG0 = (unsigned int *)0x2000030;
    unsigned int *PB_DATA = (unsigned int *)0x2000040;

    *PB_CFG0 = 1;   // config as output

    int state = 1;
    while (1) {
        state = state ^ 1;  // invert
        *PB_DATA = state;   // toggle LED on/off
        for (int count = 0x3f00000; count != 0; count--)
            ; // pause
    }
}
