#include "pi.h"

// linker memmap places these symbols at start/end of bss
extern char __bss_start__, __bss_end__;

extern void main(void);
void _cstart(void);

// The C function _cstart is called from the assembly in start.s
// _cstart zeroes out the BSS section and then calls main.
// After return from main(), turns on the green ACT LED as
// a sign of successful completion.
void _cstart(void) {
    char *bss = &__bss_start__;
    char *bss_end = &__bss_end__;

    while (bss < bss_end) {
        *bss++ = 0;
    }

    main();

    pi_led_on(PI_ACT_LED);
}
