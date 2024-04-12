
void main(void) {
    unsigned int *PB_CFG0 =  (unsigned int *)0x2000030;
    unsigned int *PB_DATA =  (unsigned int *)0x2000040;
    unsigned int *PC_CFG0  = (unsigned int *)0x2000060;
    unsigned int *PC_DATA  = (unsigned int *)0x2000070;

    *PB_CFG0 = 1; // config PB0 as output (LED)
    *PC_CFG0 = 0; // config PC0 as input (button)

    while (1) {
        if ((*PC_DATA & 1) == 0) // if button pressed
            *PB_DATA = 0; // clear PB0 (LED off)
        else
            *PB_DATA = 1; // set PB0 (LED on)
    }
}
