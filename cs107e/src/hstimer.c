/*
 * High-speed timer used as countdown with interrupt
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "hstimer.h"
#include <stdint.h>
#include "ccu.h"

// structs defined to match layout of hardware registers
typedef union {
    struct {
        uint32_t ctrl;
        uint32_t intv_lo;   // lower 32 bits of interval (see docs note above)
        uint32_t intv_hi;   // uppper 32 bits of interval
        uint32_t cur_lo;    // lower 32 bits of cur value
        uint32_t cur_hi;    // upper 32 bits of cur value
    } regs;
    unsigned char padding[0x20];
} hstimer_t;

#define TIMER_BASE ((hstimer_t *)0x3008020)
_Static_assert(&(TIMER_BASE[0].regs.ctrl)   == (uint32_t *)0x3008020, "hstimer0 ctrl reg must be at address 0x3008020");
_Static_assert(&(TIMER_BASE[1].regs.ctrl)   == (uint32_t *)0x3008040, "hstimer1 ctrl reg must be at address 0x3008040");

typedef union {
    struct {
        uint32_t irq_en;    // interrupt enable
        uint32_t irq_stas;  // interrupt status
    } regs;
} hstimer_irq_t;

#define INTERRUPT_BASE ((hstimer_irq_t *)0x3008000)
_Static_assert(&(INTERRUPT_BASE->regs.irq_stas) == (uint32_t *)0x3008004, "hstimer irq stas reg must be at address 0x3008004");

static struct {
    volatile hstimer_irq_t * interrupt;
    volatile hstimer_t *timers;
} const module = {
    .interrupt = INTERRUPT_BASE,
    .timers = TIMER_BASE,
};

/* From docs:
    HSTimer0 is a 56-bit counter. The interval value consists of two parts:
    HS_TMR0_INTV_VALUE_LO acts as the bit[31:0] and
    HS_TMR0_INTV_VALUE_HI acts as the bit[55:32].
    To read or write the interval value, HS_TMR0_INTV_LO_REG should be done before HS_TMR0_INTV_HI_REG.
 */
void hstimer_init(int usecs) {
    ccu_enable_bus_clk(CCU_HSTIMER_BGR_REG, 1 << 0, 1 << 16);   // clock up peripheral
    module.timers[0].regs.ctrl = (0 << 7) | (0 << 4);  // config for normal mode, periodic (not one-shot), prescale = 2^0, disabled
    uint64_t count = (usecs * ccu_ahb0_frequency())/(1000*1000); // calculate count based on clock frequency
    module.timers[0].regs.intv_lo = count & 0xffffffff; // must set low before high (see docs note above)
    module.timers[0].regs.intv_hi = count >> 32;
    module.timers[0].regs.ctrl |= (1 << 1);     // reload interval into cur
    module.interrupt->regs.irq_en = 1;          // enable interrupts
}

void hstimer_enable(void) {
    module.timers[0].regs.ctrl |= 1;   // enable will start (or resume) countdown
}

void hstimer_disable(void) {
    module.timers[0].regs.ctrl &= ~1; // disable will pause countdown
}

void hstimer_interrupt_clear(void) {
    module.interrupt->regs.irq_stas = 1; // write 1 to clear
}
