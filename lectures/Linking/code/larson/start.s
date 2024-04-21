# Identify this section as the one to go first in binary image
.section ".text.start"

.globl _start
_start:
    li    fp,0         # init fp to zero
    lui   sp,0x5000    # init sp to top (stack grows down)
    jal   _cstart

hang: j hang
