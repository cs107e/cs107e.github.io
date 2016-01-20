// _start is entry point
.global _start
_start:
    mov sp,#0x8000  // set up stack pointer
    bl main         // call main()
hang: b hang        // after main returns, go into infinite loop
