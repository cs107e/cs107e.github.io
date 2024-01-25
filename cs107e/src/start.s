/*
 /* File: start.s
  * -------------
  * These asm instuctions go first in binary image, they will be
  * the first to be executed in a newly loaded program.
  *
  * Author: Julie Zelenski <zelenski@cs.stanford.edu>
  */

.attribute arch, "rv64imac_zicsr"

# Identify this section as the one to go first in binary image
.section ".text.start"

.globl _start
_start:
    csrc    mstatus, 1<<3   # global disable interrupts, mstatus.mie = 0
    la      t0,mango_abort
    csrw    mtvec,t0        # install trap handler
    lui     sp,0x60000      # init stack at 0x60000000 (grows down)
    jal     _cstart

hang: j hang

.align 8
_trap_handler:
    j _trap_handler        # if exception raised, hang
