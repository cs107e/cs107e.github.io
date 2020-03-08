//
// ARM Performance Monitor support
//
// Based on
//
// Raspberry Pi memory hierarchy and access time
// Paul Drongowski
// http://sandsoftwaresound.net/raspberry-pi/raspberry-pi-gen-1/memory-hierarchy/
//

#include "pmu.h"

//
// Requested Performance Monitor events
//
int event0 = ARMV6_EVENT_NOP ;
int event1 = ARMV6_EVENT_NOP ;


//
// Return a pointer to the ASCII event name.
//
// Return NULL if the event identifier is not valid.
//
static const char* lookup_event(int event) {
    switch (event) {
        case ARMV6_EVENT_ICACHE_MISS :
            return ("Instruction cache miss") ;
        case ARMV6_EVENT_ISTALL:
            return ("Instruction buffer stall") ;
        case ARMV6_EVENT_DATA_DEPENDENT_STALL:
            return ("Data dependency stall") ;
        case ARMV6_EVENT_IMICROTLB_MISS:
            return ("Instruction MicroTLB miss") ;
        case ARMV6_EVENT_DMICROTLB_MISS:
            return ("Data MicroTLB miss") ;
        case ARMV6_EVENT_BRANCH_EXECUTED:
            return ("Executed branch instruction") ;
        case ARMV6_EVENT_BRANCH_MISPREDICT:
            return ("Mispredicted branch") ;
        case ARMV6_EVENT_INSTRUCTION_EXECUTED:
            return ("Executed instruction") ;
        case ARMV6_EVENT_DCACHE_CACHED_ACCESS:
            return ("Data cache access (cacheable)") ;
        case ARMV6_EVENT_DCACHE_ACCESS:
            return ("Data cache access") ;
        case ARMV6_EVENT_DCACHE_MISS:
            return ("Data cache miss") ;
        case ARMV6_EVENT_DCACHE_WRITEBACK:
            return ("Data cache writeback") ;
        case ARMV6_EVENT_SOFTWARE_PC_CHANGE:
            return ("Software PC change") ;
        case ARMV6_EVENT_MAIN_TLB_MISS:
            return ("Main TLB miss") ;
        case ARMV6_EVENT_EXTERNAL_DACCESS:
            return ("Explicit external data access") ;
        case ARMV6_EVENT_LSU_FULL_STALL:
            return ("Load Store Unit request queue full stall") ;
        case ARMV6_EVENT_WRITEBUFFER_DRAINED_DSB:
            return ("Write buffer drained") ;
        case ARMV6_EVENT_CPU_CYCLES:
            return ("CPU cycles") ;
        case ARMV6_EVENT_NOP:
            return ("Nop") ;
    }
    return NULL;
}

// Set events to count.
void pmu_set_events(int evt0, int evt1) {
    event0 = evt0;
    if (lookup_event(evt0) == NULL) {
        event0 = ARMV6_EVENT_NOP ;
    }

    event1 = evt1;
    if (lookup_event(evt1) == NULL) {
        event1 = ARMV6_EVENT_NOP ;
    }
}

uint32_t pmu_get_counter(int event, int *overflow) {
    uint32_t pmcr = armv6_pmcr_read() ;

    if (event == ARMV6_CYCLE_COUNTER) {
        *overflow  = (pmcr & ARMV6_PMCR_CCOUNT_OVERFLOW) != 0 ;
        return armv6pmu_read_counter(ARMV6_CYCLE_COUNTER) ;
    }

    if (event == ARMV6_COUNTER0) {
        *overflow  = (pmcr & ARMV6_PMCR_COUNT0_OVERFLOW) != 0 ;
        return armv6pmu_read_counter(ARMV6_COUNTER0) ;
    }

    if (event == ARMV6_COUNTER1) {
        *overflow  = (pmcr & ARMV6_PMCR_COUNT1_OVERFLOW) != 0 ;
        return armv6pmu_read_counter(ARMV6_COUNTER1) ;
    }

    return 0;
}

