//
// ARMv6 Performance Monitor support
//
// See section 3.2.51 in the arm1176 TRM
//
// Based on
//
// Raspberry Pi memory hierarchy and access time
// Paul Drongowski
//http://sandsoftwaresound.net/raspberry-pi/raspberry-pi-gen-1/memory-hierarchy/
//

#ifndef PMU_H
#define PMU_H

#include <stdint.h>
#ifndef NULL
#define NULL 0
#endif

#define ARMV6_EVENT_ICACHE_MISS             0x00
#define ARMV6_EVENT_ISTALL                  0x01
#define ARMV6_EVENT_DATA_DEPENDENT_STALL    0x02
#define ARMV6_EVENT_IMICROTLB_MISS          0x03
#define ARMV6_EVENT_DMICROTLB_MISS          0x04
#define ARMV6_EVENT_BRANCH_EXECUTED         0x05
#define ARMV6_EVENT_BRANCH_MISPREDICT       0x06
#define ARMV6_EVENT_INSTRUCTION_EXECUTED    0x07
#define ARMV6_EVENT_DCACHE_CACHED_ACCESS    0x09
#define ARMV6_EVENT_DCACHE_ACCESS           0x0A // including non-cached
#define ARMV6_EVENT_DCACHE_MISS             0x0B
#define ARMV6_EVENT_DCACHE_WRITEBACK        0x0C
#define ARMV6_EVENT_SOFTWARE_PC_CHANGE      0x0D
#define ARMV6_EVENT_MAIN_TLB_MISS           0x0F
#define ARMV6_EVENT_EXTERNAL_DACCESS        0x10
#define ARMV6_EVENT_LSU_FULL_STALL          0x11
#define ARMV6_EVENT_WRITEBUFFER_DRAINED_DSB 0x12
#define ARMV6_EVENT_NOP                     0x20
#define ARMV6_EVENT_CPU_CYCLES              0xFF

//
// Only the ARM1176 supports the following four events
//
#define ARMV6_EVENT_CALL_EXECUTED          0x23
#define ARMV6_EVENT_RETURN_EXECUTED        0x24
#define ARMV6_EVENT_RETURN_PREDICT         0x25
#define ARMV6_EVENT_RETURN_MISPREDICT      0x26


enum armv6_counters {
    ARMV6_CYCLE_COUNTER = 0,
    ARMV6_COUNTER0,
    ARMV6_COUNTER1,
};

//
// Read the Performance Monitor Control Register and return its value.
//
static inline uint32_t
armv6_pmcr_read(void) {
    uint32_t val;
    __asm__ volatile("mrc   p15, 0, %0, c15, c12, 0" : "=r"(val));
    return val;
}

//
// Write the Performance Monitor Control Register using the specified value.
//
static inline void
armv6_pmcr_write(uint32_t val) {
    __asm__ volatile("mcr   p15, 0, %0, c15, c12, 0" : : "r"(val));
}

//
// Read the specified Performance Monitor counter and return its value.
//
static inline uint32_t
armv6pmu_read_counter(int counter) {
    unsigned long value = 0;

    if (ARMV6_CYCLE_COUNTER == counter) {
        __asm__ volatile("mrc   p15, 0, %0, c15, c12, 1" : "=r"(value));
    } else if (ARMV6_COUNTER0 == counter) {
        __asm__ volatile("mrc   p15, 0, %0, c15, c12, 2" : "=r"(value));
    } else if (ARMV6_COUNTER1 == counter) {
        __asm__ volatile("mrc   p15, 0, %0, c15, c12, 3" : "=r"(value));
    }

    return value;
}

//
// Write a value into the  specified Performance Monitor counter.
//
static inline void
armv6pmu_write_counter(int counter,
                       uint32_t value) {
    if (ARMV6_CYCLE_COUNTER == counter) {
        __asm__ volatile("mcr   p15, 0, %0, c15, c12, 1" : : "r"(value));
    } else if (ARMV6_COUNTER0 == counter) {
        __asm__ volatile("mcr   p15, 0, %0, c15, c12, 2" : : "r"(value));
    } else if (ARMV6_COUNTER1 == counter) {
        __asm__ volatile("mcr   p15, 0, %0, c15, c12, 3" : : "r"(value));
    }
}

//
// Define bit fields within the Performance Monitor Control Register (PMCR).
//
#define ARMV6_PMCR_ENABLE               (1 << 0)
#define ARMV6_PMCR_CTR01_RESET          (1 << 1)
#define ARMV6_PMCR_CCOUNT_RESET         (1 << 2)
#define ARMV6_PMCR_CCOUNT_DIV           (1 << 3)
#define ARMV6_PMCR_COUNT0_IEN           (1 << 4)
#define ARMV6_PMCR_COUNT1_IEN           (1 << 5)
#define ARMV6_PMCR_CCOUNT_IEN           (1 << 6)
#define ARMV6_PMCR_COUNT0_OVERFLOW      (1 << 8)
#define ARMV6_PMCR_COUNT1_OVERFLOW      (1 << 9)
#define ARMV6_PMCR_CCOUNT_OVERFLOW      (1 << 10)
#define ARMV6_PMCR_EVT_COUNT0_SHIFT     20
#define ARMV6_PMCR_EVT_COUNT0_MASK      (0xFF << ARMV6_PMCR_EVT_COUNT0_SHIFT)
#define ARMV6_PMCR_EVT_COUNT1_SHIFT     12
#define ARMV6_PMCR_EVT_COUNT1_MASK      (0xFF << ARMV6_PMCR_EVT_COUNT1_SHIFT)

extern int event0;
extern int event1;

//
// Clear the performance counters and start counting.
//
static inline void pmu_start_counting(int clearcounters, int clearoverflows) {
    uint32_t pmcr = ARMV6_PMCR_ENABLE |
                    ARMV6_PMCR_CCOUNT_DIV |
                    (event0 << ARMV6_PMCR_EVT_COUNT0_SHIFT) |
                    (event1 << ARMV6_PMCR_EVT_COUNT1_SHIFT) ;

    if (clearcounters)
        pmcr |= ARMV6_PMCR_CCOUNT_RESET |
                ARMV6_PMCR_CTR01_RESET;

    if (clearoverflows)
        pmcr |= ARMV6_PMCR_CCOUNT_OVERFLOW |
                ARMV6_PMCR_COUNT0_OVERFLOW |
                ARMV6_PMCR_COUNT1_OVERFLOW ;

    armv6_pmcr_write(pmcr);
}

static inline void pmu_stop_counting(void) {
    armv6_pmcr_write(0) ;
}

void pmu_set_events(int evt0, int evt1) ;
uint32_t pmu_get_counter(int event, int *overflow);

#endif
