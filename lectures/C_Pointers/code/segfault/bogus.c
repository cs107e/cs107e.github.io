// This buggy program tries to write to a bogus address
// what happens if you run this program on a honsted system?
// what happens on baremetal Pi?

static volatile unsigned int *FSEL2 =  (unsigned int *)0x20200008;
static volatile unsigned int *SET0  =  (unsigned int *)0x2020001c;


int main(void)
{
    int *my_ptr = (int *)0x11;

    *my_ptr = 107;

    *FSEL2 = 1;         // configure GPIO 20 for output
    while (1) *SET0 = 1 << 20;    // GPIO 20 on
    return 0;
}
