/*
 * Asm routines to access CSRs used by interrupts module
 */
 .attribute arch, "rv64im_zicsr"

.globl interrupts_global_enable
interrupts_global_enable:
    li a0,1<<11         # MEIE bit 11 (m-mode external interrupts)
    csrs mie,a0         # set bit in mie register
    li a0,1<<3          # MIE bit 3 (global enable m-mode interrupts)
    csrs mstatus,a0     # set bit in mstatus register
    ret

.globl interrupts_global_disable
interrupts_global_disable:
    li a0,1<<11         # (reverse steps from global enable)
    csrc mie,a0         # clear bit in mie
    li a0,1<<3
    csrc mstatus,a0     # clear bit in mstatus
    ret

.globl interrupts_get_mcause
interrupts_get_mcause:
    csrr a0,mcause      # retrieve mcause
    ret

.globl interrupts_get_mepc
interrupts_get_mepc:
    csrr a0,mepc        # retrieve mepc
    ret

.globl interrupts_get_mtval
interrupts_get_mtval:
    csrr a0,mtval       # retrieve mtval
    ret

.globl interrupts_set_mtvec
interrupts_set_mtvec:
    csrw mtvec,a0       # mtvec holds addr of handler to call on trap
    ret
