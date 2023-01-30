const int DELAY = 0x3F0000;

static volatile unsigned int* FSEL2 = (unsigned int *)0x20200008;
static volatile unsigned int* SET0  = (unsigned int *)0x2020001c;
static volatile unsigned int* CLR0  = (unsigned int *)0x20200028;


void pause(volatile int delay)
{
    while(--delay) ;
}

void main(void)
{
    unsigned int bit_20 = 1 << 20;

    *FSEL2 = 1; // set gpio 20 to output mode

    while(1) {
        // Set GPIO 20 high (LED on)
        *SET0 = bit_20;
        pause(DELAY);
        
        // Set GPIO 20 low (LED off)
        *CLR0 = bit_20;
        pause(DELAY);
    }

}

