/* File: start.s
 * -------------
 * These asm instuctions go first in binary image, they will be
 * the first to be executed in a newly loaded program.
 */

# Identify this section as the one to go first in binary image
.section ".text.start"

.globl _start
_start:
    lui     sp,0x60      # init stack at 0x60000000 (grows down)
    call    main

hang: j hang

