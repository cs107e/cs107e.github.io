.section ".text.start"

.globl _start
_start:

.cfi_startproc
.cfi_undefined ra           # tell gdb this is entry point, has no caller
    li      fp,0            # init fp
    la      sp,__stack_top  # init sp (stack grows down)
    jal     _cstart

    hang: j hang            # backstop at end of instructions
.cfi_endproc
