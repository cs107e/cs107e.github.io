void main(void)
{
    unsigned int *FSEL2 = (unsigned int *)0x20200008;
    unsigned int *FSET0 = (unsigned int *)0x2020001c;
    *FSEL2 = 1;         // configure GPIO 20 for output
    *FSET0 = 1 << 20;   // set GPIO 20
    while (1) ;
}