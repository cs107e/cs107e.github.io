/*
 * mtimer module
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "mtimer.h"
#include "_csr_private.h"

// KEY FACT about mtimecmp
// It is a 64-bit memory mapped register but it is accessed in 32-bit words
// XuanTie-OpenC906-UserManual.pdf Section 9.1 p. 70
// See also https://lkml.org/lkml/2021/10/20/582    
static volatile uint32_t *mtimecmp = (void *)0x14004000;

void mtimer_set_mtimecmp(uint64_t when) {
    // update procedure from p. 33 of RISC-V Privileged ISA Vol II
    mtimecmp[0] = -1; // No smaller than old value
    mtimecmp[1] = when >> 32; // No smaller than new value
    mtimecmp[0] = when & 0xffffffff; // New value
}

void mtimer_advance_mtimecmp(uint64_t usecs) {
    uint64_t nticks = usecs * 24;
    mtimer_set_mtimecmp(CSR_READ(time) + nticks);
}
