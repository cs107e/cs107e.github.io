SECTIONS
{
    .text 0x8000 :  { *(.text.start) *(.text*) }
    .rodata :       { *(.rodata*) }
    .data :         { *(.data*) }
    __bss_start__ = .;
    .bss :          { *(.bss*)  *(COMMON) }
    __bss_end__ = ALIGN(8);
}

/* Force link of _start and verify correct position */
ENTRY(_start)
ASSERT(_start == ADDR(.text), "_start symbol must be placed first in text section")
