/*
 * Julie Zelenski <zelenski@cs.stanford.edu>
 * Date: Feb 20, 2020
 */
#include "assert.h"
#include "interrupts.h"
#include <stdint.h>

// http://xinu.mscs.mu.edu/BCM2835_Interrupt_Controller
// BCM: p196
// This struct is declared to match memory layout of the interrupt registers
struct interrupt_t {
    uint32_t pending_basic;
    uint32_t pending[2];
    uint32_t fiq_control;
    uint32_t enable[2];
    uint32_t enable_basic;
    uint32_t disable[2];
    uint32_t disable_basic;
};

#define INTERRUPT_CONTROLLER_BASE (void *)0x2000B200

static volatile struct interrupt_t * const interrupt = INTERRUPT_CONTROLLER_BASE;

extern uint32_t _vectors, _vectors_end;
extern uint32_t *_RPI_INTERRUPT_VECTOR_BASE;

#define MAX_HANDLERS 32

static struct {
    int irq_source;
    handler_fn_t fn;
} handlers[MAX_HANDLERS];

static int nHandlers;

void interrupts_init(void)
{
    // disable interrupt generation system-wide
    interrupts_global_disable();
    // disable all sources
    interrupt->disable_basic = 0xffffffff;
    interrupt->disable[0] = 0xffffffff;
    interrupt->disable[1] = 0xffffffff;
    // reset handler array to empty
    nHandlers = 0;
    
    // copy table of vectors to destination RPI_INTERRUPT_VECTOR_BASE
    // _vector and _vector_end are symbols defined in interrupt_asm.s
    // that mark start/end of table to be copied 
    uint32_t *dst = _RPI_INTERRUPT_VECTOR_BASE;
    uint32_t *src = &_vectors;
    uint32_t n = &_vectors_end - &_vectors;
    for (int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

// verify vector table correctly installed, i.e. interrupts_init() was
// called and IRQ table entry is not corrupted
static bool vector_table_is_installed(void)
{
    const int IRQ_INDEX = 6;
    return _RPI_INTERRUPT_VECTOR_BASE[IRQ_INDEX] == (&_vectors)[IRQ_INDEX];
}

// basic IRQ sources are ARM-specific
static bool is_basic(unsigned int irq_source)
{
    return irq_source >= INTERRUPTS_BASIC_BASE && irq_source < INTERRUPTS_BASIC_END;
}

// these IRQ sources are shared between CPU and GPU
static bool is_shared(unsigned int irq_source)
{
    return irq_source >= INTERRUPTS_SHARED_FIRST && irq_source < INTERRUPTS_SHARED_END;
}

static void enable_source(unsigned int irq_source)
{
    if (is_basic(irq_source)) {
        unsigned int shift = irq_source - INTERRUPTS_BASIC_BASE;
        interrupt->enable_basic |= 1 << shift;
    } else if (is_shared(irq_source)) {
        unsigned int bank = irq_source / 32;
        unsigned int shift = irq_source % 32;
        interrupt->enable[bank] |= 1 << shift;
    }
}

// returnes true if there is a pending event for the given source, false otherwise
// each source assigned a particular bit in one of the pending registers
static bool is_pending(unsigned int irq_source)
{
    if (is_basic(irq_source)) {
        unsigned int shift = irq_source - INTERRUPTS_BASIC_BASE;
        return interrupt->pending_basic & (1 << shift);
    } else if (is_shared(irq_source)) {
        unsigned int bank = irq_source / 32;
        unsigned int shift = irq_source % 32;
        return interrupt->pending[bank] & (1 << shift);
    }
    return false;
}

void interrupts_attach_handler(handler_fn_t fn, unsigned int source)
{
    if (!is_basic(source) && !is_shared(source)) return;

    assert(vector_table_is_installed());
    assert(nHandlers < MAX_HANDLERS);

    handlers[nHandlers].irq_source = source;
    handlers[nHandlers].fn = fn;
    nHandlers++;
    enable_source(source);
}


// The dispatch function must be extern as it is called from another module
// (interrupts_asm.s). However, it not otherwise intended as a public 
// function and is not declared or documented in the interface.
void interrupt_dispatch(unsigned int pc);


void interrupt_dispatch(unsigned int pc)
{
    for (int i = 0; i < nHandlers; i++) {
        // check handler for pending source, give a chance to process
        // if handler returns true, this indicates interrupt has
        // been processed; dispatch stops here
        // otherwise ask other attached handlers to process
        if (is_pending(handlers[i].irq_source) && handlers[i].fn(pc)) {
            return;
        }
    }
}

#if 0
Add gcc interrupt attribute to generate function with
prolog/epilog suitable for use as IRQ vector.
How does this differ from our hand-written version?

void sample_vector(void) __attribute__ ((interrupt ("IRQ")));

void sample_vector(void)
{
    interrupt_dispatch(0);
}

00000008 <sample_vector>:
   8:    sub lr, lr, #4
   c:    push    {r0, r1, r2, r3, ip, lr}
  10:    mov r0, #0
  14:    bl  0 <interrupt_dispatch>
  18:    ldm sp!, {r0, r1, r2, r3, ip, pc}^
#endif
