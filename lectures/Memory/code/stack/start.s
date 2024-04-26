.section ".text.start"

.globl _start
_start:

.cfi_startproc
.cfi_undefined ra           # tell gdb this is start routine
    addi    fp,zero,0       # init fp
    la      sp,__stack_top  # init sp (stack grows down)
    jal     _cstart

hang: j hang
    ret
.cfi_endproc
