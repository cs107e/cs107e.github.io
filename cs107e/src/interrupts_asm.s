@ Assembly code for interrupt vector table and safely setting up
@ execution in interrupt context on the Rapsberry Pi in CS107E.
@
@ Author:      Philip Levis
@ Author:      Julie Zelenski
@ Last update: 2/20/19

@ Enable/disable interrupts.
@
@ CPSR = current program status register
@        lower 8 bits are:
@           7 6 5 4 3 2 1 0
@          +-+-+-+---------+
@          |I|F|T|   Mode  |
@          +-+-+-+---------+
@
@    I     : = 0 IRQ enabled, = 1 IRQ disabled
@    F     : = 0 FIQ enabled, = 1 FIQ disabled
@    T     : = 0 indicates ARM execution, = 1 is thumb execution
@    Mode  : current mode

.global interrupts_global_enable
interrupts_global_enable:
    mrs r0, cpsr
    bic r0, r0, #0x80   @ clear I=0 enables IRQ interrupts
    msr cpsr_c, r0
    bx lr

.global interrupts_global_disable
interrupts_global_disable:
    mrs r0, cpsr
    orr r0, r0, #0x80   @ set I=1 disables IRQ interrupts
    msr cpsr_c, r0
    bx lr

        
.global _vectors
.global _vectors_end

@ Vector table has entries for each of the eight exceptions
@ Bounces to absolute destination address accessed via pc-relative label
_vectors:
    ldr pc, _abort_asm
    ldr pc, _abort_asm
    ldr pc, _abort_asm
    ldr pc, _abort_asm
    ldr pc, _abort_asm
    ldr pc, _abort_asm
    ldr pc, _interrupt_asm
    ldr pc, _abort_asm
    
    _abort_asm:                   .word abort_asm
    _interrupt_asm:               .word interrupt_asm

_vectors_end:

abort_asm:
    mov sp, #0x4000                  @ set up stack on the fly
    bl pi_abort

interrupt_asm:
    sub   lr, lr, #4                @ compute return address
    push  {r0-r3, r12, lr}          @ save registers
    mov   r0, lr                    @ pass old pc as argument
    bl    interrupt_vector          @ call C function
    ldm   sp!, {r0-r3, r12, pc}^    @ return, ^ to change mode + restore cpsr

.global _RPI_INTERRUPT_VECTOR_BASE
    _RPI_INTERRUPT_VECTOR_BASE:     .word   0
