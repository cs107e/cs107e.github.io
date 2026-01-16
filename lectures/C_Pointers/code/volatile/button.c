void main(void) {
    unsigned int *PB_CFG0 =  (unsigned int *)0x2000030;
    unsigned int *PB_DATA =  (unsigned int *)0x2000040;
    unsigned int *PC_CFG0  = (unsigned int *)0x2000060;
    unsigned int *PC_DATA  = (unsigned int *)0x2000070;

    *PB_CFG0 = 1;   // config PB0 output (LED)
    *PC_CFG0 = 0;   // config PC0 input (button)
    *PB_DATA = 1;   // LED on

    while ((*PC_DATA & 1) != 0) // loop until button pressed
        ;
    *PB_DATA = 0;   // LED off
}
