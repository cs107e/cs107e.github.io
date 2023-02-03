// Identify this section as the one to go first in binary image
.section ".text.start"

.globl _start
_start:
    mov sp, #0x8000000
    mov fp, #0
    bl _cstart
hang: b hang
