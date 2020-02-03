#include "malloc.h"

// Super simple "bump allocator"
// calls sbrk to enlarge heap segment for each allocation
// never recycles memory (free is a no-op)


// Round up x to next multiple of n. Requires
// that n be a power of two to work correctly
#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))


// privately tracks address for heap_end
// advances to accommodate requested number of bytes
void *sbrk(int nbytes)
{
    extern int __bss_end__;
    static void *heap_end = &__bss_end__;

    void *prev_end = heap_end;
    heap_end = (char *)heap_end + nbytes;
    return prev_end;
}


void *malloc(size_t nbytes) 
{
    nbytes = roundup(nbytes, 8);
    return sbrk(nbytes);
}

void free(void *ptr) 
{
    // move along, nothing to see here
}

