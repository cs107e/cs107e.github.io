/*
 /* File: start.s
  * -------------
  * These asm instuctions go first in binary image, they will be
  * the first to be executed in a newly loaded program.
  *
  * Author: Julie Zelenski <zelenski@cs.stanford.edu>
  */

.attribute arch, "rv64im_zicsr"

# Identify this section to linker script memmap.ld
.section ".text.start"

.type _start, @function
.globl _start

.cfi_startproc
.cfi_undefined ra           # tell gdb this is entry point, has no caller

_start:
    csrc    mstatus, 1<<3   # global disable interrupts, mstatus.mie = 0
    la      t0,_trap_handler
    csrw    mtvec,t0        # install trap handler
.globl _start_gdb
_start_gdb:                 # gdb entry set here to skip over csr insns that are unavail in sim
    li      fp,0            # init fp
    la      sp,__stack_top  # init sp (stack grows down)
    jal     _cstart

    hang: j hang            # backstop at end of instructions

.cfi_endproc
.size _start, .-_start
