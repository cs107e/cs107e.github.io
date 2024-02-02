
extern void main(void);

void _cstart(void) {

    extern unsigned char __bss_start, __bss_end;
    for (unsigned char *cur = &__bss_start; cur < &__bss_end; cur++) {
        *cur = 0;
    }

    main();
}
