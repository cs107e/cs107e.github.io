/*
 * Implementation of ringbuffer module.
 *
 * The ring buffer data structure allows lock-free concurrent
 * access by one reader and one writer.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 *         Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "ringbuffer.h"
#include "assert.h"
#include "malloc.h"

#define CAPACITY 512

/*
 * A ring buffer is represented using a struct containing a fixed-size array
 * and head and tail fields, which are indexes into the entries[] array.
 * head is the index of the frontmost element (head advances during dequeue)
 * tail is the index of the next position to use (tail advances during enqueue)
 * Both head and tail advance circularly, i.e. index = (index + 1) % CAPACITY
 * The ring buffer is empty if tail == head
 * The ring buffer is full if tail + 1 == head
 * (one slot permanently unused to distinguish full from empty)
 */

struct ringbuffer {
    int entries[CAPACITY];
    int head, tail;
};

rb_t *rb_new(void) {
    rb_t *rb = malloc(sizeof *rb);
    if (!rb) return NULL;
    rb->head = rb->tail = 0;
    return rb;
}

bool rb_empty(rb_t *rb) {
    assert(rb != NULL);
    return rb->head == rb->tail;
}

bool rb_full(rb_t *rb) {
    assert(rb != NULL);
    return (rb->tail + 1) % CAPACITY == rb->head;
}

/*
 * enqueue (writer) advances rb->tail, no changes to rb->head.
 * This design allows safe concurrent access.
 */
void rb_enqueue(rb_t *rb, int elem) {
    assert(rb != NULL);
    assert(!rb_full(rb));

    rb->entries[rb->tail] = elem;
    rb->tail = (rb->tail + 1) % CAPACITY;
}

/*
 * dequeue (reader) advances rb->head, no changes to rb->tail.
 * This design allows safe concurrent access.
 */
int rb_dequeue(rb_t *rb) {
    assert(rb != NULL);
    assert(!rb_empty(rb));

    int front = rb->entries[rb->head];
    rb->head = (rb->head + 1) % CAPACITY;
    return front;
}
