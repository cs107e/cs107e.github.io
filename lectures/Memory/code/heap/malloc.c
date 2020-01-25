#include "malloc.h"

// A simple "bump allocator" that 
// just increments the heap segment to allocate space
// never recycles memory (free is a no-op)


// Round up x to next multiple of n. Requires
// that n be a power of two to work correctly
#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))


extern int __bss_end__;

static void *heap_end = &__bss_end__;

void *malloc(size_t nbytes) 
{
    nbytes = roundup(nbytes, 8);
    void *alloc = heap_end;
    heap_end = (char *)heap_end + nbytes;
    return alloc;
}

void free(void *ptr) 
{
    // move along, nothing to see here
}

