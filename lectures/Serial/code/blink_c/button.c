static unsigned int *FSEL1 =  (unsigned int *)0x20200004;
static unsigned int *FSEL2 =  (unsigned int *)0x20200008;
static unsigned int *SET0  =  (unsigned int *)0x2020001c;
static unsigned int *LEV0  =  (unsigned int *)0x20200034;

void main(void)
{
    unsigned int bit_10 = 1 << 10;
    unsigned int bit_20 = 1 << 20;

    *FSEL1 = 0; // bit 10 is input pin
    *FSEL2 = 1; // bit 20 is output pin

	// Wait until GPIO 10 is low (button press)
	while ((*LEV0 & bit_10) != 0) ;

	// Set GPIO 20 high (LED on)
	*SET0 = bit_20;
}
