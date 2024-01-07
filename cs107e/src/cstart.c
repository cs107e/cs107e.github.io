/*
 * File: cstart.c
 * --------------
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "mango.h"

// linker memmap places these symbols at start/end of bss
extern char __bss_start__, __bss_end__;

extern void main(void);
void _cstart(void);

// The C function _cstart is called from the assembly in start.s
// _cstart zeroes out the BSS section and then calls main.
void _cstart(void) {
    char *bss = &__bss_start__;
    char *bss_end = &__bss_end__;

    while (bss < bss_end) {
        *bss++ = 0;
    }

    mango_actled(LED_ON);   // turn on blue onboard LED while executing main
    main();
    mango_reboot();         // reset the Pi if completed main() successfully
}
