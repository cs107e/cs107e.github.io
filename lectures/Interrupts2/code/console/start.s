// The reference full assembly start sequence for CS107E.
// This sets up the stack for both interrupt and supervisor
// modes before branching to _cstart (in cstart.c). 
//
// If _cstart() returns, _start hangs in an infinite loop.
//
// Author: Philip Levis
// Last update: 2/20/2019

.globl _start
_start:
    mov r0, #0xD2     @ mode = interrupt
    msr cpsr_c, r0
    mov sp, #0x8000

    mov r0, #0xD3     @ mode = supervisor
    msr cpsr_c, r0      
    mov sp, #0x8000000
    mov fp, #0

    bl _cstart

hang: b hang
