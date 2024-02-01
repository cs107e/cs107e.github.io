/*
 * File: cstart.c
 * --------------
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "mango.h"
#include "strings.h"

extern void main(void);
void _cstart(void);
void *sys_memset(void *s, int c, size_t n);

// The C function _cstart is called from the assembly in start.s
// _cstart zeroes out the BSS section and then calls the main function
void _cstart(void) {
    // linker script memmap.ld places symbols to mark bss boundaries
    extern char __bss_start, __bss_end;
    sys_memset(&__bss_start, 0, &__bss_end - &__bss_start);

    mango_actled(LED_ON);   // turn on blue onboard LED while executing main
    main();
    mango_reboot();         // reset the Pi if main() completed successfully
}
