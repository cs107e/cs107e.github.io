/*
 * Functions to manage D1 clocks (bus and PLL)
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Mon Jan  1 12:03:30 PST 2024
 */

#include "ccu.h"
#include "assert.h"

#define CCU_BASE 0x2001000L

#define CCU(offset) *(volatile uint32_t *)(CCU_BASE + offset)

void ccu_write(uint32_t reg, uint32_t val) {
    CCU(reg) = val;
}

uint32_t ccu_read(uint32_t reg) {
    return CCU(reg);
}

void ccu_enable_pll(uint32_t reg, uint32_t M, uint32_t N) {
    assert(M <= 0xff && N <= 1);
    CCU(reg) |= (1 << 31); // PLL_en
    CCU(reg) &= ~(1 << 27); // disable output while changing
    CCU(reg) &= ~((0xff << 8) | (0x1 << 1)); // clear M & N
    CCU(reg) |= (M << 8) | (N << 1);
    CCU(reg) |= (1 << 29); // lock enable
    while (! (CCU(reg) & (1 << 28))); // wait til stable
    CCU(reg) |= 1 << 27; // enable output
}


/*
 * From docs:
    For the Bus Gating Reset register of a module, the reset bit is de-asserted first,
    and then the clock gating bit is enabled to avoid potential problems
    caused by the asynchronous release of the reset signal.
 */
void ccu_enable_bus_clk(uint32_t reg, uint32_t gating, uint32_t reset) {
    CCU(reg) |= reset;      // de-assert reset
    CCU(reg) |= gating;     // set gating to enable
}


uint64_t ccu_ahb0_frequency(void) {
    return 200*1000*1000;
}
