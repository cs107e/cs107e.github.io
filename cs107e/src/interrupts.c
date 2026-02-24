/*
 * Use of PLIC to configure and dispatch external interrupts
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * June 2023
 */

#include "interrupts.h"
#include "assert.h"
#include "disassemble.h"
#include "mango.h"
#include <stddef.h>
#include "_system.h"

/* Module-private helpers defined in interrupts_asm.s */
unsigned long interrupts_get_mepc(void);
unsigned long interrupts_get_mcause(void);
unsigned long interrupts_get_mtval(void);
void interrupts_set_mtvec(void *);

#define N_SOURCES 256
// structs defined to match layout of hardware registers
typedef union {
    struct {
        uint32_t priority[1024];    // only first 256 indexes are used (priority per source)
        uint32_t pending[1024];     // only first 8 indexes used (pending bit per source, 256/32 = 8)
        uint32_t enable[1024];      // only first 8 indexes used (enable bit per source, 256/32 = 8)
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

#define DESCRIPTION(cause) (cause < sizeof(table)/sizeof(*table) ? table[cause] : "Unknown")

void _trap_handler(void);

// gcc attribute used to generate prologue/epilogue appropriate for machine interrupt
// https://gcc.gnu.org/onlinedocs/gcc/RISC-V-Function-Attributes.html
__attribute__((interrupt("machine"))) void _trap_handler(void) {
    long mcause = interrupts_get_mcause();
#define EXTERNAL_INTERRUPT ((1L << 63) | 0xb)
    if (mcause == EXTERNAL_INTERRUPT) { // trap is interrupt
        // do not need to search pending bits to identify which source, claim reg has source number
        uint32_t source = module.plic->regs.claim_complete; // read claim_complete to "claim" (atomically clears pending bit)
        module.handlers[source].fn(module.handlers[source].aux_data); // dispatch to registered handler
        module.plic->regs.claim_complete = source;   // write claim_complete to clear
    } else { // trap is exception
        long mtval = interrupts_get_mtval();
        void *mepc = (void *)interrupts_get_mepc();
        sys_report_error("EXCEPTION: %s\n", DESCRIPTION(mcause));
        if (mcause == 0 || mcause == 1) {
            sys_report_error("Cannot fetch instruction at invalid address (mepc):  %p\n", mepc);
        } else if (mcause == 2 || mcause == 3) {
            sys_report_error("Faulting instruction (mepc):   [%pW] %pI at %p %pL\n", mepc, mepc, mepc, mepc);
        } else if (mcause == 5 || mcause == 7) {
            sys_report_error("Instruction (mepc): %p %pL\n", mepc, mepc);
            sys_report_error("Faulting address (mtval): 0x%lx \n", mtval);
        } else {
            sys_report_error("Instruction (mepc): %p %pL\n", mepc, mepc);
            sys_report_error("Value (mtval):  %8ld   0x%lx \n", mtval, mtval);
        }
        mango_abort();
    }
}

void interrupts_init(void) {
    if (module.initialized) error("interrupts_init() must be called only once");
    interrupts_global_disable();
    module.plic->regs.ctrl = 0;             // machine mode only
    module.plic->regs.threshhold = 0;       // accept interrupts of any priority
    interrupts_set_mtvec(_trap_handler);    // install trap handler
    for (int i = 0; i < N_SOURCES/32; i++) { // 1 bit per souce
        module.sources->regs.pending[i] = 0;// all sources initially disabled
        module.sources->regs.enable[i] = 0;
    }
    for (int i = 0; i < N_SOURCES; i++) {
        module.sources->regs.priority[i] = 0;
        module.handlers[i].fn = NULL;           // all sources initially have NULL handler
        module.plic->regs.claim_complete = i;   // reset any pending claim
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
        module.sources->regs.enable[bank] |= (1 << shift); // set enable bit
    } else {
        module.sources->regs.priority[source] = 0;
        module.sources->regs.enable[bank] &= ~(1 << shift); // clear enable bit
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
    module.handlers[source].fn = fn;                // store handler function and
    module.handlers[source].aux_data = aux_data;    // aux_data pointer into array at index corresponding to source
}
