/*
 * Use of of plic to configure and dispatch external interrupts
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * June 2023
 */

#include "interrupts.h"
#include "assert.h"
#include "_csr_private.h"
#include "mango.h"
#include "printf.h"

// structs defined to match layout of hardware registers
typedef union {
    struct {
        uint32_t priority[1024];    // only first 256 in use
        uint32_t pending[1024];     // only first 9 in use
        uint32_t enable[1024];      // only first 9 in use
    } regs;
} source_t;

#define SOURCE_BASE ((source_t *)0x10000000)
_Static_assert(&(SOURCE_BASE->regs.priority[0]) == (uint32_t *)0x10000000, "priority0 reg must be at address 0x10000000");
_Static_assert(&(SOURCE_BASE->regs.pending[1])  == (uint32_t *)0x10001004, "pending1 reg must be at address 0x10001004");

typedef union {
    struct {
        uint32_t ctrl;
        uint32_t threshhold;
        uint32_t claim;
    } regs;
} plic_t;

#define PLIC_BASE ((plic_t *)0x101FFFFC)
_Static_assert(&(PLIC_BASE->regs.claim)       == (uint32_t *)0x10200004, "plic claim reg must be at address 0x10200004");

#define N_SOURCES 256

static struct {
    source_t *sources;
    plic_t *plic;
    struct {
        handlerfn_t fn;
        void *aux_data;
    } handlers[N_SOURCES];
    bool initialized;
} module = {
    .sources = SOURCE_BASE,
    .plic =    PLIC_BASE,
};

//  gcc attribute will generate prologue/epilogue appropriate for machine interrupt
__attribute__((interrupt("machine"))) static void interrupt_dispatch(void) {
    uint64_t external_interrupt = ((1L << 63) | 11);
    if (CSR_READ(mcause) != external_interrupt) {
        error("TRAP: mcause %lx mip %lx, mepc %lx mtval %lx\n", CSR_READ(mcause), CSR_READ(mip), CSR_READ(mepc), CSR_READ(mtval));
    }
    uint32_t source = module.plic->regs.claim; // read claim register start processing
    if (module.handlers[source].fn) {
        module.handlers[source].fn(CSR_READ(mepc), module.handlers[source].aux_data);
    } else {
        error("UNHANDLED external interrupt on source %d\n", source);
    }
    module.plic->regs.claim = source;   // write claim register to complete
}

static void confirm_interrupts_initialized(const char *);

void interrupts_init(void) {
    confirm_interrupts_initialized(NULL);
    interrupts_global_disable();
    module.plic->regs.ctrl = 0; // machine mode only
    module.plic->regs.threshhold = 0; // accept interrupts of any priority
    CSR_WRITE(mtvec, &interrupt_dispatch);
    for (int i = 0; i < 9; i++) {
        module.sources->regs.pending[i] = 0;
        module.sources->regs.enable[i] = 0;
    }
    for (int i = 0; i < N_SOURCES; ++i) {
        module.sources->regs.priority[i] = 0;
        module.handlers[i].fn = NULL;
        module.plic->regs.claim = i; // complete any pending request
    }
    module.initialized = true;
}

void interrupts_global_enable(void) {
    confirm_interrupts_initialized(__func__);
    CSR_SET_BIT(mstatus, 1 << 3); // global enable machine mode interrupts MIP mstatus bit[3]
    CSR_SET_BIT(mie, 1 << 11); // enable machine mode external interrupts MEIP mie bit[11]
}

void interrupts_global_disable(void) {
    CSR_CLEAR_BIT(mstatus, 1 << 3);
    CSR_CLEAR_BIT(mie, 1 << 11);
}

static bool is_valid_source(int source) {
    return source >= 1 && source < N_SOURCES;
}

void interrupts_register_handler(int source, handlerfn_t fn, void *aux_data) {
#warning need separation between register handler and enable/disable interrupt source
    confirm_interrupts_initialized(__func__);
    if (!is_valid_source(source)) error("%d is not a valid interrupt source", source);
    if (fn == NULL) error("cannot register NULL handler");
    module.handlers[source].fn = fn;
    module.handlers[source].aux_data = aux_data;
    int bank = source / 32;
    int shift = source % 32;
    module.sources->regs.priority[source] = 1; // priority at 1 (0 is invalid, 1 is lowest)
    module.sources->regs.enable[bank] |= (1 << shift);
}

void interrupts_remove_handler(int source) {
    confirm_interrupts_initialized(__func__);
    if (!is_valid_source(source)) error("%d is not a valid interrupt source", source);
    module.handlers[source].fn = NULL;
    module.handlers[source].aux_data = NULL;
    int bank = source / 32;
    int shift = source % 32;
    module.sources->regs.priority[source] = 0;
    module.sources->regs.enable[bank] &= ~(1 << shift);
}

static void confirm_interrupts_initialized(const char *fn_name) {
    if (module.initialized && fn_name == NULL) {
        error("interrupts_init() must be called only once");
    } else if (!module.initialized && fn_name != NULL) {
        error("interrupts_init() must be called before using %s()", fn_name);
    }
}
