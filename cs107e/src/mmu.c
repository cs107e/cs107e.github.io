#include "memory.h"
#include "system.h"
#include "mmu.h"

void mmu_section(unsigned base, unsigned vaddr, unsigned paddr, unsigned flags) {
    // map from vaddr to tbl addr
    unsigned ra = vaddr >> 20;
    unsigned rb = base | (ra << 2);
    unsigned rc = (paddr & 0xFFF00000) | MMU_PAGE_ACCESS_RW | MMU_PAGE_SECTION | flags;
    PUT32(rb, rc);
}

/*
** initialize the mmu
**
** base is the physical address of the page table
**
** the base should point to a buffer containing 0x4000 (4096) ints
*/
void mmu_init(unsigned base) {
    //memory
    for (unsigned ra = 0;; ra += 0x00100000) {
        mmu_section(base, ra, ra, MMU_PAGE_CACHEABLE | MMU_PAGE_BUFFERABLE);
        //if(ra==0xFFF00000) break;
        if (ra == 0x10000000) {
            break;    // 256MB
        }
    }

    //peripherals
    mmu_section(base, 0x20000000, 0x20000000, 0x00000); //NOT CACHED!
    mmu_section(base, 0x20200000, 0x20200000, 0x00000); //NOT CACHED!

    system_start_mmu(base,
                     SYSTEM_MMU_ENABLE |
                     SYSTEM_DCACHE_ENABLE |
                     SYSTEM_ICACHE_ENABLE |
                     SYSTEM_BRANCH_PREDICTION_ENABLE |
                     SYSTEM_WRITE_BUFFER_ENABLE);
}
