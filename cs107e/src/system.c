#include "system.h"

unsigned
system_get_cache_type(void) {
    // See section 3.3.2 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c0, c0, 1" : "=r"(reg));
    return reg;
}

void
system_set_cache_control(unsigned reg) {
    // See section 3.2.7 in arm1176
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

unsigned system_get_cache_control(void) {
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    return reg;
}

void
system_enable_dcache(void) {
    // See section 3.2.7 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg |= SYSTEM_DCACHE_ENABLE;
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

void
system_disable_dcache(void) {
    // See section 3.2.7 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg &= ~SYSTEM_DCACHE_ENABLE;
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

void
system_enable_icache(void) {
    // See section 3.2.7 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg |= SYSTEM_ICACHE_ENABLE;
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

void
system_disable_icache(void) {
    // See section 3.2.7 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg &= ~SYSTEM_ICACHE_ENABLE;
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

void
system_enable_write_buffer(void) {
    // See section 3.2.7 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg |= SYSTEM_WRITE_BUFFER_ENABLE;
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

void
system_disable_write_buffer(void) {
    // See section 3.2.7 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg &= ~SYSTEM_WRITE_BUFFER_ENABLE;
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

void
system_enable_branch_prediction(void) {
    // See section 3.2.7 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg |= SYSTEM_BRANCH_PREDICTION_ENABLE;
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

void
system_disable_branch_prediction(void) {
    // See section 3.2.7 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg &= ~SYSTEM_BRANCH_PREDICTION_ENABLE;
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

void
system_enable_cache(void) {
    // See section 3.2.7 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg |= SYSTEM_DCACHE_ENABLE |
           SYSTEM_ICACHE_ENABLE |
           SYSTEM_WRITE_BUFFER_ENABLE |
           SYSTEM_BRANCH_PREDICTION_ENABLE;
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

void
system_disable_cache(void) {
    // See section 3.2.7 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg &= ~(SYSTEM_DCACHE_ENABLE |
             SYSTEM_ICACHE_ENABLE |
             SYSTEM_WRITE_BUFFER_ENABLE |
             SYSTEM_BRANCH_PREDICTION_ENABLE);
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

void
system_enable_mmu(void) {
    // See section 3.2.7 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg |= SYSTEM_MMU_ENABLE;
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

void
system_disable_mmu(void) {
    // See section 3.2.7 in arm1176
    unsigned reg;
    __asm__ volatile("mrc   p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg &= ~SYSTEM_MMU_ENABLE;
    __asm__ volatile("mcr   p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

// system_random_replacement()
// system_round_round_replacement()

void
system_invalidate_icache(void) {
    // See Fig 3.38 in section 3.2.22 in arm1176
    __asm__ volatile("mcr   p15, 0, %0, c7, c5, 0" : : "r"(0));
}

void
system_invalidate_dcache(void) {
    // See Fig 3.38 in section 3.2.22 in arm1176
    __asm__ volatile("mcr   p15, 0, %0, c7, c6, 0" : : "r"(0));
}

void
system_invalidate_cache(void) {
    // See Fig 3.38 in section 3.2.22 in arm1176
    __asm__ volatile("mcr   p15, 0, %0, c7, c7, 0" : : "r"(0));
}

void
system_clean_dcache(void) {
    // See Fig 3.38 in section 3.2.22 in arm1176
    __asm__ volatile("mcr   p15, 0, %0, c7, c10, 0" : : "r"(0));
}

void
system_clean_cache(void) {
    // See Fig 3.38 in section 3.2.22 in arm1176
    __asm__ volatile("mcr   p15, 0, %0, c7, c11, 0" : : "r"(0));
}

void
system_clean_and_invalidate_dcache(void) {
    // See Fig 3.38 in section 3.2.22 in arm1176
    __asm__ volatile("mcr   p15, 0, %0, c7, c14, 0" : : "r"(0));
}

void
system_clean_and_invalidate_cache(void) {
    // See Fig 3.38 in section 3.2.22 in arm1176
    system_clean_and_invalidate_dcache();
    system_invalidate_icache();
}

/**
 * @fn void dmb(void)
 *
 * Executes a data memory barrier operation using the c7 (Cache Operations)
 * register of system control coprocessor CP15.
 *
 * All explicit memory accesses occurring in program order before this operation
 * will be globally observed before any memory accesses occurring in program
 * order after this operation.  This includes both read and write accesses.
 *
 * This differs from a "data synchronization barrier" in that a data
 * synchronization barrier will ensure that all previous explicit memory
 * accesses occurring in program order have fully completed before continuing
 * and that no subsequent instructions will be executed until that point, even
 * if they do not access memory.  This is unnecessary for what we need this for.
 *
 * On the BCM2835 (Raspberry Pi), this is needed before and after accessing
 * peripherals, as documented on page 7 of the "BCM2835 ARM Peripherals"
 * document.  As documented, it is only needed when switching between
 * _different_ peripherals.
 */

void
system_invalidate_tlb(void) {
    __asm__ volatile("mcr   p15, 0, %0, c8, c7, 0" : : "r"(0));
    dsb();
}


//
// armv6 has 16 different domain
//
// each domain is stored as 2-bits
//
void
system_set_domain(unsigned reg) {
    __asm__ volatile("mcr   p15, 0, %0, c3, c0, 0" : : "r"(reg));
}

void
system_set_tlb_base(unsigned base) {
    __asm__ volatile("mcr   p15, 0, %0, c2, c0, 0" : : "r"(base));
    __asm__ volatile("mcr   p15, 0, %0, c2, c0, 1" : : "r"(base));
}

void
system_start_mmu(unsigned base, unsigned flags) {
    system_invalidate_cache();
    system_invalidate_tlb();
    dsb();

    system_set_domain(~0); // don't check access

    system_set_tlb_base(base);

    system_set_cache_control(flags);
}

void
system_stop_mmu(void) {
    system_invalidate_tlb();
    system_disable_mmu();
}
