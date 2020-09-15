#ifndef RINGBUFFER_REF
#define RINGBUFFER_REF
#include "ringbuffer.h"
rb_t *ref_rb_new(void);
bool ref_rb_empty(rb_t *rb);
bool ref_rb_full(rb_t *rb);
bool ref_rb_enqueue(rb_t *rb, int elem);
bool ref_rb_dequeue(rb_t *rb, int *p_elem);
#endif
