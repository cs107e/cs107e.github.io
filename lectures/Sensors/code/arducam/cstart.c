// linker memmap places these symbols at start/end of bss
extern int __bss_start__, __bss_end__;

extern void main(void);

// The C function _cstart is called from the assembly in start.s
// _cstart zeroes out the BSS section and then calls main.
// After main() completes, turns on the green ACT LED as
// a sign of successful execution.
void _cstart(void) 
{
    int *bss = &__bss_start__;
    int *bss_end = &__bss_end__;

    while (bss < bss_end) {
        *bss++ = 0;
    }

    main();

    // Turn on the green ACT LED (GPIO 47)
    volatile unsigned int *GPIO_FSEL4  = (unsigned int *)0x20200010;
    volatile unsigned int *GPIO_SET1   = (unsigned int *)0x20200020;
    *GPIO_FSEL4 = (1 << ((47%10)*3));
    *GPIO_SET1 =  (1 << (47%32));
}
