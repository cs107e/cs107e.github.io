#include "memmap.h"

void _cstart(void) {
    for (unsigned char *cur = &__bss_start; cur < &__bss_end; cur++) {
        *cur = 0;
    }
    main();
}
