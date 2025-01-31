.section ".text.start"

.globl _start
_start:
    lui   sp,0x50000    # init sp to top 0x50000000 (stack grows down)
    jal   _cstart

    hang: j hang        # backstop at end of instructions
