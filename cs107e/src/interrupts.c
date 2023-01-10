/*
 * Module to configure/enable/dispatch interrupts at top-level
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 *         Julie Zelenski <zelenski@cs.stanford.edu>
 *
 * Last modified: Feb 2022
 */
#include "assert.h"
#include "bits.h"
#include "interrupts.h"
#include <stdbool.h>
#include <stddef.h>
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
static const uint64_t irq_safe_mask = (1ULL << INTERRUPTS_AUX)       |
        (1ULL << INTERRUPTS_I2CSPISLV) |
        (1ULL << INTERRUPTS_PWA0)      |
        (1ULL << INTERRUPTS_PWA1)      |
        (1ULL << INTERRUPTS_CPR)       |
        (1ULL << INTERRUPTS_SMI)       |
        (1ULL << INTERRUPTS_GPIO0)     |
        (1ULL << INTERRUPTS_GPIO1)     |
        (1ULL << INTERRUPTS_GPIO2)     |
        (1ULL << INTERRUPTS_GPIO3)     |
        (1ULL << INTERRUPTS_VC_I2C)    |
        (1ULL << INTERRUPTS_VC_SPI)    |
        (1ULL << INTERRUPTS_VC_I2SPCM) |
        (1ULL << INTERRUPTS_VC_UART);

extern uint32_t _vectors, _vectors_end;
extern uint32_t *_RPI_INTERRUPT_VECTOR_BASE;

// This array stores the handlers for peripheral interrupts (IRQs) and as well
// as basic interrupts. The 2835 has 64 IRQs, these are entries 0-63. We
// place the basic interrupts after them (see the enumeration in
// interrupts.h). If no handler is registered for a given interrupt source,
// the fn at that index is NULL.
static struct {
    handler_fn_t fn;
    void *aux_data;
} handlers[INTERRUPTS_COUNT];

static bool interrupts_initialized = false;

void interrupts_init(void) {
    // init should happen once and only once
    // A re-init would disable all previously enabled sources
    // raise error rather than do that.
    assert(!interrupts_initialized);

    // disable interrupt generation system-wide
    interrupts_global_disable();
    // disable all sources
    interrupt->disable_basic = 0xffffffff;
    interrupt->disable[0] = 0xffffffff;
    interrupt->disable[1] = 0xffffffff;

    // copy table of vectors to destination RPI_INTERRUPT_VECTOR_BASE
    // _vector and _vector_end are symbols defined in interrupt_asm.s
    // that mark start/end of table to be copied
    uint32_t *dst = _RPI_INTERRUPT_VECTOR_BASE;
    uint32_t *src = &_vectors;
    uint32_t n = &_vectors_end - &_vectors;
    for (int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    interrupts_initialized = true;
}

// verify vector table correctly installed, i.e. interrupts_init() was
// called and IRQ table entry is not corrupted
static bool vector_table_is_installed(void) {
    const int IRQ_INDEX = 6;
    return _RPI_INTERRUPT_VECTOR_BASE[IRQ_INDEX] == (&_vectors)[IRQ_INDEX];
}

// basic IRQ sources are ARM-specific
static bool is_basic(unsigned int irq_source) {
    return irq_source >= INTERRUPTS_BASIC_BASE && irq_source < INTERRUPTS_BASIC_END;
}

// these IRQ sources are shared between CPU and GPU
static bool is_shared(unsigned int irq_source) {
    return irq_source >= INTERRUPTS_SHARED_START && irq_source < INTERRUPTS_SHARED_END;
}

// Returns whether a given IRQ is safe to be used.
// From the BCM2835 manual 7.5: "the table above has many empty entries.
// These should not be enabled as they will interfere with the GPU operation."
// Returns whether the irq is a non-empty entry, and thus safe to be enabled.
static bool is_safe(unsigned int irq_source) {
    uint64_t bit = 1ULL << irq_source;
    return ((bit & irq_safe_mask) != 0);
}

static bool is_valid(unsigned int irq_source) {
    return is_basic(irq_source) || (is_shared(irq_source) && is_safe(irq_source));
}

void interrupts_enable_source(unsigned int source) {
    assert(interrupts_initialized);
    assert(is_valid(source));

    if (is_basic(source)) {
        unsigned int shift = source - INTERRUPTS_BASIC_BASE;
        interrupt->enable_basic |= 1 << shift;
    } else if (is_shared(source)) {
        unsigned int bank = source / 32;
        unsigned int shift = source % 32;
        interrupt->enable[bank] |= 1 << shift;
    }
}

void interrupts_disable_source(unsigned int source) {
    assert(interrupts_initialized);
    assert(is_valid(source));

    if (is_basic(source)) {
        unsigned int shift = source - INTERRUPTS_BASIC_BASE;
        interrupt->disable_basic |= 1 << shift;
    } else if (is_shared(source)) {
        unsigned int bank = source / 32;
        unsigned int shift = source % 32;
        interrupt->disable[bank] |= 1 << shift;
     }
}

void interrupts_register_handler(unsigned int source, handler_fn_t fn, void *aux_data) {
    assert(interrupts_initialized);
    assert(vector_table_is_installed());
    assert(is_valid(source));

    handlers[source].fn = fn;
    handlers[source].aux_data = aux_data;
}

static int get_next_source(void) {
    unsigned int basic_zeroes = count_leading_zeroes(interrupt->pending_basic);
    unsigned int pending0_zeroes = count_leading_zeroes(interrupt->pending[0]);
    unsigned int pending1_zeroes = count_leading_zeroes(interrupt->pending[1]);
    // We only care about basic bits 0-7:
    //  - bits 8 and 9 are for the pending registers, which we always check
    //  - higher bits are GPU which we ignore
    if (basic_zeroes >= 24) {
        // If bit 0 is set there will be 31 leading zeroes, while if bit 31 is set
        // there are 0 leading zeroes. So the number is 31 - number of zeroes; add
        // INTERRUPTS_BASIC_BASE for index into table.
        return (31 - basic_zeroes) + INTERRUPTS_BASIC_BASE;
    } else if (pending0_zeroes != 32) {
        return (31 - pending0_zeroes);
    } else if (pending1_zeroes != 32) {
        return (63 - pending1_zeroes);
    } else {
        return INTERRUPTS_NONE;
    }
}

// The dispatch function must be extern as it is called from another module
// (interrupts_asm.s). However, it not otherwise intended as a public
// function and is not declared or documented in the interface.
void interrupt_dispatch(unsigned int pc);

void interrupt_dispatch(unsigned int pc) {
    int source = get_next_source();
    if (source < INTERRUPTS_COUNT && handlers[source].fn != NULL) {
        handlers[source].fn(pc, handlers[source].aux_data);
    }
}

