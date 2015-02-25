.section .text
.globl _start
_start:
    mov r0, #0xD2    @ IRQ mode
    msr cpsr_c, r0   @ Put in IRQ mode, don't clear C bits
    mov sp, #0x8000  @ Set IRQ stack pointer
    mov r0, #0xD3    @ SVC mode
    msr cpsr_c, r0   @ Put in SVC mode, don't clear C bits
    mov sp, #0x7000  @ Set SVC stack pointer
    bl _cstart       @ Jump to C start routine

hang:
    b hang


