void main(void) {
    unsigned int *FSEL2 = (unsigned int *)0x20200008;
    unsigned int *SET0 = (unsigned int *)0x2020001C;

    // config GPIO 20 as output (LED)
    *FSEL2 = 1;

    // set GPIO 20 (LED on)
    *SET0 = 1 << 20;

    while (1) ;
}
