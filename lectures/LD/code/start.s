.globl start
start:
    mov sp,#0x8000
    bl cstart
hang: 
    b hang
