// This buggy program tries to write to a bogus address (0x3)
// what happens if you run this program on a hosted system?
// what happens if you run on bare metal Pi?

static volatile unsigned int *FSEL2 =  (unsigned int *)0x20200008;
static volatile unsigned int *SET0  =  (unsigned int *)0x2020001c;


int main(void)
{
    int *p = (int *)0x3;

    *p = 107;

    // now turn on GPIO 20, does program get here?
    *FSEL2 = 1;         // configure GPIO 20 for output
    *SET0 = 1 << 20;    // turn GPIO 20 on
    return 0;
}
