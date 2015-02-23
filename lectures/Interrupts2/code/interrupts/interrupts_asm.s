.globl _table
.globl _table_end
        
_table:
  ldr pc, _reset_asm
  ldr pc, _undefined_instruction_asm
  ldr pc, _software_interrupt_asm
  ldr pc, _prefetch_abort_asm
  ldr pc, _data_abort_asm
  ldr pc, _reset_asm
  ldr pc, _interrupt_asm
fast_interrupt_asm:
  sub   lr, lr, #4 @First instr of FIQ handler
  push  {lr}
  push  {r0-r12}
  mov   r0, lr              @ Pass old pc
  bl    fast_interrupt_vector    @ C function
  pop   {r0-r12}
  ldm   sp!, {pc}^

_reset_asm:                   .word reset_asm
_undefined_instruction_asm:   .word undefined_instruction_asm
_software_interrupt_asm:      .word software_interrupt_asm
_prefetch_abort_asm:          .word prefetch_abort_asm
_data_abort_asm:              .word data_abort_asm
_interrupt_asm:               .word interrupt_asm

_table_end:

interrupt_asm:
  sub   lr, lr, #4
  push  {lr}
  push  {r0-r12}
  mov   r0, lr              @ Pass old pc
  bl    interrupt_vector    @ C function
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

