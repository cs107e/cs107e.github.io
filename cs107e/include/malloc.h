#ifndef MALLOC_H
#define MALLOC_H

/*
 * Functions for dynamic allocation.
 *
 * Students implement this module in assignment 4.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stddef.h> // for size_t

/*
 * `malloc`
 *
 * Service a dynamic allocation request. Returns the
 * address of a memory block of at least `nybtes` contiguous
 * bytes or NULL if the request cannot be satisifed.
 * The block address is guaranteed to be aligned to an 8-byte boundary.
 * If `nbytes` is 0, malloc returns either NULL or a unique
 * pointer to an allocated block of some minimum size.
 *
 * @param nbytes    requested size in bytes
 * @return          address of memory block of requested size
 *                  or NULL if request cannot be satisifed
 */
void *malloc(size_t nbytes);

/*
 * `free`
 *
 * Deallocate the memory block at address `ptr`.
 *
 * The `ptr` argument is expected to be an address that was
 * previously returned by malloc and has not yet been freed.
 * If this precondition is not satisified, the behavior is undefined.
 * If `ptr` is NULL, the operation does nothing.
 *
 * @param ptr       address of memory block to deallocate
 */
void free(void *ptr);

/*
 * `sbrk`
 *
 * Return the address of the previous end of the heap segment
 * and enlarge the segment by the specified number of bytes.
 * The call `sbrk(0)` returns the address of the end of segment
 * with no change in size. You may assume that no calls to
 * sbrk with a non-zero argument will come from outside clients,
 * calls to enlarge the segment will only come from within
 * your heap allocator implementation.
 *
 * @param nbytes    the number of bytes to extend the segment end by
 * @return          address of previous end of segment (before enlarge)
 */
void *sbrk(size_t nbytes);

#endif
