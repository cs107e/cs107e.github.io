static unsigned int *FSEL1 =  (unsigned int *)0x20200004;
static unsigned int *FSEL2 =  (unsigned int *)0x20200008;
static unsigned int *SET0  =  (unsigned int *)0x2020001c;
static unsigned int *CLR0  =  (unsigned int *)0x20200028;
static unsigned int *LEV0  =  (unsigned int *)0x20200034;

void main(void) {
    *FSEL1 = 0; // config GPIO 10 as input (button)
    *FSEL2 = 1; // config GPIO 20 as output (LED)

    *SET0 = 1 << 20; // set GPIO 20 (LED on)

    while ((*LEV0 & (1 << 10)) != 0) // while button not pressed, wait
        ;

    *CLR0 = 1 << 20; // clear GPIO 20 (LED off)
}
