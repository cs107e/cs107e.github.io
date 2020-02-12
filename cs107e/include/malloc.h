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
 * The returned pointer is guaranteed to be aligned to an 8-byte boundary.
 * If `nbytes` is 0, malloc returns either NULL or a unique 
 * pointer to an allocated block of some minimum size.
 */
void *malloc(size_t nbytes);

/* Function: free
 * --------------
 * Deallocates the memory at address `ptr`. 
 *
 * The `ptr` argument is expected to be an address that was previously
 * return by malloc and has not yet been freed. If this 
 * precondition is not satisified, the behavior is undefined.
 * If `ptr` is NULL, the operation does nothing.
 */
void free(void *ptr);

/* Function: realloc
 * -----------------
 * Changes the size of the memory block pointed to by `ptr` to at least
 * `new_size` bytes. The contents of the memory block are unchanged
 * from the start to the minimum of the old and new sizes. If
 * requested change in size cannot be accommodated in-place,
 * realloc() creates a new allocation, copies as much of the data 
 * pointed to by `ptr` as will fit to the new allocation, frees
 * the previous allocation, and returns a pointer to the new memory.
 *
 * The `ptr` argument is expected to an address that was
 * previously returned by malloc and has not yet been freed. 
 * If this precondition is not satisified, the behavior is undefined.
 *
 * The call realloc(NULL, size) is equivalent to malloc(size)
 * The call realloc(ptr, 0) is equivalent to free(ptr) and returns NULL
 */
void *realloc(void *ptr, size_t new_size);

#endif
