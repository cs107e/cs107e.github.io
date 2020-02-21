.global _RPI_INTERRUPT_VECTOR_BASE
    _RPI_INTERRUPT_VECTOR_BASE:     .word   0

.global _vectors
.global _vectors_end

@ Buggy vector table using pc-relative branch -- offset will be
@ incorrect when table copied to different address
_vectors:
    b abort_asm
    b abort_asm
    b abort_asm
    b abort_asm
    b abort_asm
    b abort_asm
    b interrupt_asm
_vectors_end:

abort_asm:
    b    abort_asm                  @ hang

interrupt_asm:
    mov   sp, #0x8000               @ init stack for interrupt mode
    sub   lr, lr, #4                @ compute return address
    push  {r0-r3, r12, lr}          @ save registers
    mov   r0, lr                    @ pass old pc as argument
    bl    interrupt_vector          @ call C function
    ldm   sp!, {r0-r3, r12, pc}^    @ return, ^ to change mode + restore cpsr
