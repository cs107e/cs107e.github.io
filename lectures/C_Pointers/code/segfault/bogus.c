#include <stddef.h>
// This buggy program tries to write to a bogus address (NULL, zero pointer)
// what happens if you run this program on a hosted system?
// what happens if you run bare-metal on Pi?

int main(void) {
    unsigned int *PB_CFG0 = (unsigned int *)0x2000030;
    unsigned int *PB_DATA = (unsigned int *)0x2000040;

    int *p = NULL;

    *p = 107; // dereference NULL? not a good idea...

    *PB_CFG0 = 1;       // config PB0 as output (LED)
    *PB_DATA = 1;       // turn on LED (shows that execution got here)

    while (1) ;
    return 0;
}
