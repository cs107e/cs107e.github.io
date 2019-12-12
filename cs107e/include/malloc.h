#ifndef MALLOC_H
#define MALLOC_H

/*
 * Functions for dynamic allocation.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Mon Feb  5 20:02:27 PST 2018
 */
#include <stddef.h> // for size_t


/* Function: malloc
 * ----------------
 * Services a dynamic allocation request. Returns the 
 * address of a block of at least `nybtes` contiguous bytes
 * or NULL if the request cannot be satisifed.
 */
void *malloc(size_t nbytes);

/* Function: free
 * --------------
 * Deallocates the memory at address `ptr`. 
 *
 * The `ptr` argument is expected to an address that was previously
 * return by malloc and has not yet been freed. If this 
 * precondition is not satisified, the behavior is undefined.
 */
void free(void *ptr);

/* Function: realloc
 * -----------------
 * Resizes the memory allocated for `ptr` to `new_size`. If the
 * requested change in size cannot be accommodated in-place,
 * realloc() creates a new allocation, copies as much of the data 
 * pointed to by `ptr` as will fit to the new allocation, frees
 * the previous allocation, and returns a pointer to the new memory.
 *
 * The `ptr` argument is expected to an address that was
 * previously return by malloc and has not yet been freed. 
 * If this precondition is not satisified, the behavior is undefined.
 */
void *realloc(void *ptr, size_t new_size);

#endif
