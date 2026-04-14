#include "malloc.h"
#include "memmap.h"

// Super simple "bump allocator"
// calls sbrk to enlarge heap segment for each allocation
// never recycles memory (free is a no-op)


// Round up x to next multiple of n. Requires
// that n be a power of two to work correctly
#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))


// Call sbrk as needed to extend size of heap segment
// Use sbrk implementation as given
void *sbrk(size_t nbytes) {
    // marker placed by linker script memmap.ld
    static void *cur_heap_end = HEAP_START;

    void *prev_heap_end = cur_heap_end;
    cur_heap_end = (char *)cur_heap_end + nbytes;
    return prev_heap_end;
}

void *malloc(size_t nbytes) {
    nbytes = roundup(nbytes, 8);
    return sbrk(nbytes);
}

void free(void *ptr) {
    // move along, nothing to see here
}

