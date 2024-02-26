/*
 * Use of PLIC to configure and dispatch external interrupts
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * June 2023
 */

#include "interrupts.h"
#include "assert.h"
#include "mango.h"
#include <stddef.h>
#include "_system.h"

/* Module-private helpers defined in interrupts_asm.s */
unsigned long get_mepc(void);
unsigned long get_mcause(void);
unsigned long get_mtval(void);
void set_mtvec(void *);

#define N_SOURCES 256
// structs defined to match layout of hardware registers
typedef union {
    struct {
        uint32_t priority[1024];    // only first 256 used (N_SOURCES)
        uint32_t pending[1024];     // only first 8 used (one pending bit per source)
        uint32_t enable[1024];      // only first 8 used (one enable bit per source)
    } regs;
} source_t;

#define SOURCE_BASE ((source_t *)0x10000000)
_Static_assert(&(SOURCE_BASE->regs.priority[0]) == (uint32_t *)0x10000000, "priority0 reg must be at address 0x10000000");
_Static_assert(&(SOURCE_BASE->regs.pending[1])  == (uint32_t *)0x10001004, "pending1 reg must be at address 0x10001004");

typedef union {
    struct {
        uint32_t ctrl;
        uint32_t threshhold;
        uint32_t claim_complete;
    } regs;
} plic_t;

#define PLIC_BASE ((plic_t *)0x101FFFFC)
_Static_assert(&(PLIC_BASE->regs.claim_complete)   == (uint32_t *)0x10200004, "plic claim reg must be at address 0x10200004");

static struct {
    volatile source_t *sources;
    volatile plic_t *plic;
    struct {
        handlerfn_t fn;
        void *aux_data;
    } handlers[N_SOURCES];
    bool initialized;
} module = {
    .sources = SOURCE_BASE,
    .plic =    PLIC_BASE,
};

static const char *description(unsigned int cause) {
    static const char *table[] = {
        "Instruction address misaligned",
        "Instruction access fault",
        "Illegal instruction",
        "Breakpoint",
        "Load address misaligned",
        "Load access fault",
        "Store/AMO address misaligned",
        "Store/AMO access fault",
        "E-call from U-mode",
        "E-call from S-mode",
        "Reserved (10)",
        "E-call from M-mode", /* external interrupt, M-mode */
        "Instruction page fault",
        "Load page fault",
        "Reserved (14)",
        "Store/AMO page fault",
    };
    int n = sizeof(table)/sizeof(*table);
    return (cause < n ? table[cause] : "Unknown");
}

// gcc attribute used to generate prologue/epilogue appropriate for machine interrupt
// https://gcc.gnu.org/onlinedocs/gcc/RISC-V-Function-Attributes.html
__attribute__((interrupt("machine"))) static void trap_handler(void) {
#define EXTERNAL_INTERRUPT ((1L << 63) | 0xb)
    if (get_mcause() == EXTERNAL_INTERRUPT) {
        // no need to search pending bits to identify source, claim reg has it
        uint32_t source = module.plic->regs.claim_complete; // read claim_complete to "claim" (atomically clears pending bit)
        module.handlers[source].fn(get_mepc(), module.handlers[source].aux_data); // dispatch to registered handler
        module.plic->regs.claim_complete = source;   // write claim_complete to "complete"
    } else {
        sys_report_error("EXCEPTION: %s (mtval 0x%lx, mepc 0x%lx)\n", description(get_mcause()), get_mtval(), get_mepc());
        mango_abort();
    }
}

void interrupts_init(void) {
    if (module.initialized) error("interrupts_init() must be called only once");
    interrupts_global_disable();
    module.plic->regs.ctrl = 0;         // machine mode only
    module.plic->regs.threshhold = 0;   // accept interrupts of any priority
    set_mtvec(trap_handler);            // install trap handler
    for (int i = 0; i < 8; i++) {       // all sources start disabled
        module.sources->regs.pending[i] = 0;
        module.sources->regs.enable[i] = 0;
    }
    for (int i = 0; i < N_SOURCES; i++) {
        module.sources->regs.priority[i] = 0;
        module.handlers[i].fn = NULL;
        module.plic->regs.claim_complete = i; // mark any pending request completed
    }
    module.initialized = true;
}

static bool is_valid_source(interrupt_source_t source) {
    switch (source) {
        case INTERRUPT_SOURCE_UART0...INTERRUPT_SOURCE_UART5:
        case INTERRUPT_SOURCE_TWI0...INTERRUPT_SOURCE_TWI3:
        case INTERRUPT_SOURCE_SPI0...INTERRUPT_SOURCE_SPI1:
        case INTERRUPT_SOURCE_HSTIMER0...INTERRUPT_SOURCE_HSTIMER1:
        case INTERRUPT_SOURCE_GPIOB...INTERRUPT_SOURCE_GPIOG:
            return true;
    }
    return false;
}

static void set_source_enabled(interrupt_source_t source, bool enabled) {
    if (!module.initialized) error("interrupts_init() has not been called!\n");
    if (!is_valid_source(source)) error("request to enable/disable interrupt source that is not valid");
    int bank = source / 32;
    int shift = source % 32;
    if (enabled) {
        module.sources->regs.priority[source] = 1; // priority at 1 (0 is disable, 1 is lowest)
        module.sources->regs.enable[bank] |= (1 << shift);
    } else {
        module.sources->regs.priority[source] = 0;
        module.sources->regs.enable[bank] &= ~(1 << shift);
    }
}

void interrupts_enable_source(interrupt_source_t source) {
    set_source_enabled(source, true);
}

void interrupts_disable_source(interrupt_source_t source) {
    set_source_enabled(source, false);
}

void interrupts_register_handler(interrupt_source_t source, handlerfn_t fn, void *aux_data) {
    if (!module.initialized) error("interrupts_init() has not been called!\n");
    if (!is_valid_source(source)) error("request to register handler for interrupt source that is not valid");
    module.handlers[source].fn = fn;
    module.handlers[source].aux_data = aux_data;
}
