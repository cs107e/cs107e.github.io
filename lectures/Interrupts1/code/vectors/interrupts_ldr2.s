.globl abort_asm
.globl interrupt_asm

abort_asm:
    b    abort_asm                  @ hang

interrupt_asm:
    mov   sp, #0x8000               @ init stack for interrupt mode
    sub   lr, lr, #4                @ compute resume addr from old pc
    push  {r0-r12, lr}              @ save all registers (overkill, but simple & correct)
    mov   r0, lr                    @ pass resume addr as argument to dispatch
    bl    interrupt_dispatch        @ call C function
    ldm   sp!, {r0-r12, pc}^        @ return from interrupt mode
                                    @ restore saved regs (note pc = lr/resume addr)
                                    @ The ^ changes mode & restores cpsr
