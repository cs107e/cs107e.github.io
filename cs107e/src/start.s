/*
 /* File: start.s
  * -------------
  * These asm instuctions go first in binary image, they will be
  * the first to be executed in a newly loaded program.
  *
  * Author: Julie Zelenski <zelenski@cs.stanford.edu>
  */

.attribute arch, "rv64im_zicsr"

# Identify this section as the one to go first in binary image
.section ".text.start"

.globl _start
_start:
.cfi_startproc
.cfi_undefined ra           # tell gdb this is start routine

    csrc    mstatus, 1<<3   # global disable interrupts, mstatus.mie = 0
    la      t0,mango_abort
    csrw    mtvec,t0        # install trap handler
.globl _start_gdb
_start_gdb:                 # entry for gdb will skip csr as not avail in sim
    addi    fp,zero,0       # init fp
    la      sp,__stack_top  # init sp (stack grows down)
    jal     _cstart

hang: j hang
    ret
.cfi_endproc

.align 8
_trap_handler:
    j _trap_handler        # if exception raised, hang
