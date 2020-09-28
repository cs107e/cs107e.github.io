// Make volatile to allow optimizations on
static unsigned int *FSEL1 =  (unsigned int *)0x20200004;
static unsigned int *FSEL2 =  (unsigned int *)0x20200008;
static unsigned int *SET0  =  (unsigned int *)0x2020001c;
static unsigned int *CLR0  =  (unsigned int *)0x20200028;
static unsigned int *LEV0  =  (unsigned int *)0x20200034;

void main(void) {
    const unsigned int bit_10 = 1 << 10;
    const unsigned int bit_20 = 1 << 20;

    *FSEL1 = 0; // bit 10-19 are input
    *FSEL2 = 1; // bit 20 an output

    while (1) {
	// Wait until GPIO 10 is low (button press)
	while ((*LEV0 & bit_10) != 0) ;

	// Set GPIO 20 high (LED on)
	*SET0 = bit_20;

	// Wait until GPIO 10 is high (button release)
	while ((*LEV0 & bit_10) == 0) ;

	// Set GPIO 20 low (LED off)
	*CLR0 = bit_20;
    }

}
