.global _start
_start:
    mov sp,#0x8000
    bl newmain
hang: 
    b hang

