.globl _start

_start:
    mov sp, #0x8000
    bl _cstart

hang:
    bl led_on
    b .

