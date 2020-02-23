@ Assembly code for interrupt vector table and functions
@ to enable/disable IRQ interrupts on the Rapsberry Pi in CS107E.
@
@ Author:      Philip Levis
@ Author:      Julie Zelenski
@ Last update: 2/20/20

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
    mrs r0, cpsr        @ copy cpsr value to regular register
    bic r0, r0, #0x80   @ clear I=0 enables IRQ interrupts
    msr cpsr_c, r0      @ copy back, control bits only
    bx lr

.global interrupts_global_disable
interrupts_global_disable:
    mrs r0, cpsr        @ copy cpsr value to regular register
    orr r0, r0, #0x80   @ set I=1 disables IRQ interrupts
    msr cpsr_c, r0      @ copy back, control bits only
    bx lr

@ destination address for vector table (0)
.global _RPI_INTERRUPT_VECTOR_BASE
    _RPI_INTERRUPT_VECTOR_BASE:     .word   0

.global _vectors
.global _vectors_end

@ Vector table has one instruction for each of the eight exceptions
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
    mov   sp, #0x4000               @ init stack
    bl    pi_abort                  @ pi_abort does not return

interrupt_asm:
    mov   sp, #0x8000               @ init stack for interrupt mode
    sub   lr, lr, #4                @ compute resume addr from old pc
    push  {r0-r12, lr}              @ save all registers (overkill, but simple & correct)
    mov   r0, lr                    @ pass resume addr as argument to dispatch
    bl    interrupt_dispatch        @ call C function
    ldm   sp!, {r0-r12, pc}^        @ leave interrupt mode
	                                @ restore saved regs, pc restored to lr (resume addr)
	                                @ The ^ changes to previous mode, restores cpsr
