.global _RPI_INTERRUPT_VECTOR_BASE
    _RPI_INTERRUPT_VECTOR_BASE:     .word   0

.global _vectors
.global _vectors_end

@ Vector table has entries for each of the eight exceptions
@ Bounces to absolute destination address accessed via pc-relative label
_vectors:
    ldr pc, abort_addr
    ldr pc, abort_addr
    ldr pc, abort_addr
    ldr pc, abort_addr
    ldr pc, abort_addr
    ldr pc, abort_addr
    ldr pc, interrupt_addr
    ldr pc, abort_addr
    
    abort_addr:                   .word abort_asm
    interrupt_addr:               .word interrupt_asm

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
