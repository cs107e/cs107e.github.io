static volatile unsigned int *FSEL2 =  (unsigned int *)0x20200008;
static volatile unsigned int *SET0  =  (unsigned int *)0x2020001c;
static volatile unsigned int *CLR0  =  (unsigned int *)0x20200028;

#define DELAY 0x3f0000

void main(void)
{
   *FSEL2 = 1;  // configure GPIO 20 for output

   while (1) {
        *SET0 = 1 << 20;    // set GPIO 20
        for (int c = DELAY; c != 0; c--) ;  // wait
        *CLR0 = 1 << 20;    // clear GPIO 20
        for (int c = DELAY; c != 0; c--) ;  // wait
   }
}

