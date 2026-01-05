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
        uint32_t intv_hi;   // upper 32 bits of interval
        uint32_t cur_lo;    // lower 32 bits of cur value
        uint32_t cur_hi;    // upper 32 bits of cur value
    } regs;
    uint8_t padding[0x20];
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
    volatile hstimer_irq_t *interrupt;
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
void hstimer_init(hstimer_id_t index, long usecs) {
    if (index != HSTIMER0 && index != HSTIMER1) return;
    long rate = ccu_ungate_bus_clock(CCU_HSTIMER_BGR_REG);  // clock up peripheral
    module.timers[index].regs.ctrl = (0 << 7) | (0 << 4);   // config mode normal periodic (not one-shot), prescale = 2^0, not enabled
    uint64_t count = (usecs * rate)/(1000*1000);            // calculate count based on clock frequency
    module.timers[index].regs.intv_lo = count & 0xffffffff; // must set low before high (see docs note above)
    module.timers[index].regs.intv_hi = count >> 32;
    module.timers[index].regs.ctrl |= (1 << 1);             // reload interval into cur
    module.interrupt->regs.irq_en |= (1 << index);          // enable interrupts
}

void hstimer_enable(hstimer_id_t index) {
    if (index != HSTIMER0 && index != HSTIMER1) return;
    module.timers[index].regs.ctrl |= 1;   // set ctrl bit will start/resume countdown
}

void hstimer_disable(hstimer_id_t index) {
    if (index != HSTIMER0 && index != HSTIMER1) return;
    module.timers[index].regs.ctrl &= ~1; // clear ctrl bit will pause countdown
}

void hstimer_interrupt_clear(hstimer_id_t index) {
    if (index != HSTIMER0 && index != HSTIMER1) return;
    module.interrupt->regs.irq_stas = (1 << index); // write 1 to clear
}
