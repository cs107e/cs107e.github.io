#ifndef CSR_H
#define CSR_H

/*
 * Macros for zicsr instructions (Control and Status Registers)
 *
 * Must add _zicsr to ARCH flags to enable csr instructions
 * see https://www.five-embeddev.com/riscv-isa-manual/latest/priv-csrs.html
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdint.h>

/*
 * In the macros below, the csr argument can either be a a compile-time
 * constant (12-bit integer) or name mnemonic (i.e. cycle, time, mtvec, mepc)
 */

#define CSR_READ(csr)                      \
({ uint64_t v;                             \
__asm__ __volatile__ ("csrr %0, " #csr     \
              : "=r" (v)                   \
              : /* no inputs */            \
              : /* clobbers: none */ );    \
    v;})

#define CSR_WRITE(csr, v)                   \
__asm__ __volatile__ ("csrw " #csr " , %0"  \
              : /* no outputs */            \
              : "rK" (v)                    \
              : /* clobbers: none */ );

#define CSR_SET_BIT(csr, v)                 \
__asm__ __volatile__ ("csrs " #csr " , %0"  \
              : /* no outputs */            \
              : "rK" (v)                    \
              : /* clobbers: none */ );

#define CSR_CLEAR_BIT(csr, v)               \
__asm__ __volatile__ ("csrc " #csr " , %0"  \
              : /* no outputs */            \
              : "rK" (v)                    \
              : /* clobbers: none */ );

#endif
