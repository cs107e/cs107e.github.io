#ifndef RINGBUFFER_H
#define RINGBUFFER_H

/*
 * This module defines a ring buffer data structure that provides
 * a fixed-length FIFO (first-in-first-out) queue of int elements.
 *
 * The queue is designed to allow concurrent access by one reader (rb_dequeue)
 * and one writer (rb_enqueue). The interrupt handler (writer) enqueues
 * data to be dequeued by the main program (reader).
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>

/*
 * Type: `rb_t`
 *
 * This struct holds the information for a ring buffer.
 */
typedef struct ringbuffer rb_t;

/*
 * `rb_new`
 *
 * Initializes a new empty ring buffer and returns a pointer to it.
 *
 * @return      pointer to new ring buffer or NULL if failed to create
 *
 * To set up a ring buffer in your code:
 *
 *     rb_t *rb = rb_new();
 *
 * Notice the interface is slightly different than other library modules
 * which provide a module_init routine, intended to be called just once to
 * initialize the module. A module_new function can be called more than once,
 * to allocate a new distinct ring buffer on each call, like creating new
 * objects in Java. The client uses the returned pointer to a rb_t without
 * knowing the inner details which are privately encapsulated within the .c file.
 */
rb_t *rb_new(void);

/*
 * `rb_empty`
 *
 * Check if a ring buffer is currently empty, i.e. contains
 * no elements.
 *
 * @param rb    the ring buffer to check
 * @return      true if rb is empty, false otherwise
 */
bool rb_empty(rb_t *rb);

/*
 *  `rb_full`
 *
 * Check if a ring buffer is currently full to capacity, i.e.
 * no further elements can be enqueued until some are dequeued
 * to make space.
 *
 * @param rb    the ring buffer to check
 * @return      true if rb is full, false otherwise
 */
bool rb_full(rb_t *rb);

/*
 * `rb_enqueue`
 *
 * Add an element to the back of a ring buffer. It is an error to
 * attempt to enqueue to a ring buffer that is full to capacity.
 * Use `!rb_full(rb)` to confirm before enqueueing.
 *
 * @param rb    the ring buffer to enqueue to
 * @param elem  the element to enqueue to back
 */
void rb_enqueue(rb_t *rb, int elem);

/*
 * `rb_dequeue`
 *
 * Remove frontmost element and return it. It is an error to
 * attempt to dequeue from an empty ring buffer.
 * Use `!rb_empty(rb)` to confirm before dequeueing.
 *
 * @param rb        the ring buffer to dequeue from
 * @return          the element dequeued from front
 */
int rb_dequeue(rb_t *rb);

#endif
