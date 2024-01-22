#include <stddef.h>
// This buggy program tries to write to a bogus address (NULL, zero pointer)
// what happens if you run this program on a hosted system?
// what happens if you run on bare metal Pi?

int main(void) {
    unsigned int *PB_CFG0 = (unsigned int *)0x2000030;
    unsigned int *PB_DATA = (unsigned int *)0x2000040;

    int *p = NULL;

    *p = 107; // dereference NULL? not a good idea...

    // config PB0 as output (LED)
    *PB_CFG0 = 1;

    // set PB0 (LED on)
    *PB_DATA = 1;

    while (1) ;
    return 0;
}
