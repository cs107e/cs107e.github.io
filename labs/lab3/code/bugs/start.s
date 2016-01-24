.global _start
_start:
    mov sp, #0x8000
    bl notmain
hang:
    b hang
