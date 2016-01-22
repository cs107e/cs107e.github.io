// _start is entry point
.global _start
_start:
    mov sp,#0x8000  // set up stack pointer
    mov fp,#0x0		// init frame pointer to zero
    bl main         // call main()
hang: b hang        // after main returns, go into infinite loop
