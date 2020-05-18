.global _RPI_INTERRUPT_VECTOR_BASE
    _RPI_INTERRUPT_VECTOR_BASE:     .word   0

.global _vectors
.global _vectors_end

@ Buggy vector table using explicit load into pc  -- 
@ constants will be embedded but where? Have to
@ copy them too.
_vectors:
    ldr pc, =abort_asm 
    ldr pc, =abort_asm 
    ldr pc, =abort_asm 
    ldr pc, =abort_asm 
    ldr pc, =abort_asm 
    ldr pc, =abort_asm 
    ldr pc, =interrupt_asm
    ldr pc, =abort_asm 
_vectors_end:

