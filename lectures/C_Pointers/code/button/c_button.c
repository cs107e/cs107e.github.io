static volatile unsigned int *FSEL1 =  (unsigned int *)0x20200004;
static volatile unsigned int *FSEL2 =  (unsigned int *)0x20200008;
static volatile unsigned int *SET0  =  (unsigned int *)0x2020001c;
static volatile unsigned int *CLR0  =  (unsigned int *)0x20200028;
static volatile unsigned int *LEV0  =  (unsigned int *)0x20200034;

void main(void)
{
   *FSEL1 = 0;  // configure GPIO 10 for input
   *FSEL2 = 1;  // configure GPIO 20 for output
   int bit_10 = 1 << 10;
   int bit_20 = 1 << 20;

   while (1) {
        if ((*LEV0 & bit_10) == 0)  // if button pressed, input is low
            *SET0 = bit_20;    // set GPIO 20
        else
            *CLR0 = bit_20;    // clear GPIO 20
   }
}