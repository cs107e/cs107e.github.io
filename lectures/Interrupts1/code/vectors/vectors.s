.globl _vectors
.globl _vectors_end
        
_vectors:
    ldr pc, _abort_asm
    ldr pc, _abort_asm
    ldr pc, _abort_asm
    ldr pc, _abort_asm
    ldr pc, _abort_asm
    ldr pc, _abort_asm
    ldr pc, _interrupt_asm
    ldr pc, _abort_asm

    _abort_asm:         .word abort_asm
    _interrupt_asm:     .word interrupt_asm
_vectors_end:

interrupt_asm:
    sub   lr, lr, #4                @ compute return address
    push  {r0-r3, r12, lr}          @ save registers
    mov   r0, lr                    @ pass old pc as argument
    bl    interrupt_vector          @ call C function
    ldm   sp!, {r0-r3, r12, pc}^    @ return, ^ to change mode + restore cpsr

abort_asm:
    b abort_asm

.global _RPI_INTERRUPT_VECTOR_BASE
    _RPI_INTERRUPT_VECTOR_BASE:     .word   0
