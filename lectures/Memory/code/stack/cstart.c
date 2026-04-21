#include "memmap.h"

void _cstart(void) {
    for (uint8_t *bss = BSS_START; bss < BSS_END; bss++) {
        *bss = 0;
    }
    main();
}
