.globl _table
        
_table:
ldr pc, =reset_asm
ldr pc, =undefined_instruction_asm
ldr pc, =software_interrupt_asm
ldr pc, =prefetch_abort_asm
ldr pc, =data_abort_asm
ldr pc, =reset_asm
ldr pc, =interrupt_asm
ldr pc, =fast_interrupt_asm

interrupt_asm:
  sub   lr, lr, #4
  push  {lr}
  push  {r0-r12}
  mov   r0, lr              @ Pass old pc
  bl    interrupt_vector    @ C function
  pop   {r0-r12}
  ldm   sp!, {pc}^

fast_interrupt_asm:
  sub   lr, lr, #4
  push  {lr}
  push  {r0-r12}
  mov   r0, lr              @ Pass old pc
  bl    fast_interrupt_vector    @ C function
  pop   {r0-r12}
  ldm   sp!, {pc}^

reset_asm:
  sub   lr, lr, #4
  push  {lr}
  push  {r0-r12}
  mov   r0, lr              @ Pass old pc
  bl    reset_vector        @ C function
  pop   {r0-r12}
  ldm   sp!, {pc}^

undefined_instruction_asm:
  sub   lr, lr, #4
  push  {lr}
  push  {r0-r12}
  mov   r0, lr                          @ Pass old pc
  bl    undefined_instruction_vector    @ C function
  pop   {r0-r12}
  ldm   sp!, {pc}^

software_interrupt_asm:
  sub   lr, lr, #4
  push  {lr}
  push  {r0-r12}
  mov   r0, lr                       @ Pass old pc
  bl    software_interrupt_vector    @ C function
  pop   {r0-r12}
  ldm   sp!, {pc}^

prefetch_abort_asm:
  sub   lr, lr, #4
  push  {lr}
  push  {r0-r12}
  mov   r0, lr                   @ Pass old pc
  bl    prefetch_abort_vector    @ C function
  pop   {r0-r12}
  ldm   sp!, {pc}^

data_abort_asm:
  sub   lr, lr, #4
  push  {lr}
  push  {r0-r12}
  mov   r0, lr                 @ Pass old pc
  bl    data_abort_vector      @ C function
  pop   {r0-r12}
  ldm   sp!, {pc}^

