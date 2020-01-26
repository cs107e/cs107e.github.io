#ifndef SYSTEM_H
#define SYSTEM_H

/*
 * Hardware abstractions for caches and other memory features
 * of the Raspberry Pi processor.
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Date: Mar 1, 2016
 */
unsigned system_get_cache_type(void);

#define SYSTEM_MMU_ENABLE                (1<<0)
#define SYSTEM_DCACHE_ENABLE             (1<<2)
#define SYSTEM_WRITE_BUFFER_ENABLE       (1<<3)
#define SYSTEM_BRANCH_PREDICTION_ENABLE  (1<<11)
#define SYSTEM_ICACHE_ENABLE             (1<<12)
#define SYSTEM_ROUND_ROBIN_ENABLE        (1<<14)
void system_set_cache_control(unsigned reg);
unsigned system_get_cache_control(void);

void system_enable_cache(void);
void system_disable_cache(void);
void system_enable_dcache(void);
void system_disable_dcache(void);
void system_enable_icache(void);
void system_disable_icache(void);

void system_invalidate_cache(void);
void system_clean_cache(void);
void system_clean_and_invalidate_cache(void);

void system_invalidate_dcache(void);
void system_clean_dcache(void);
void system_clean_and_invalidate_dcache(void);

void system_invalidate_icache(void);

void system_enable_write_buffer(void);
void system_disable_write_buffer(void);

void system_enable_branch_prediction(void);
void system_disable_branch_prediction(void);

void system_invalidate_tlb(void);

// access not allowed
#define SYSTEM_DOMAIN_NO_ACCESS       0b00
// user and priv access permission check
#define SYSTEM_DOMAIN_CHECK_ACCESS    0b01
// user access permission check, priv permission access not checked
#define SYSTEM_DOMAIN_NO_CHECK_ACCESS 0b11
void system_set_domain(unsigned reg);

void system_set_tlb_base(unsigned base);

void system_start_mmu(unsigned base, unsigned flags);
void system_stop_mmu(void);
void system_enable_mmu(void);
void system_disable_mmu(void);

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

/*
 * Data memory barrier
 *
 * No memory access after the DMB can run until
 * all memory accesses before it have completed
 */
#define dmb() __asm__ volatile \
        ("mcr p15, #0, %0, c7, c10, #5" : : "r" (0) )


/*
 * Data synchronisation barrier
 *
 * No instruction after the DSB can run until
 * all instructions before it have completed
 */
#define dsb() __asm__ volatile \
        ("mcr p15, #0, %0, c7, c10, #4" : : "r" (0) )


#endif
