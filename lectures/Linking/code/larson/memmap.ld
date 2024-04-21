SECTIONS
{
    .text 0x40000000  :  { *(.text.start) *(.text*) }
    .rodata           :  { *(.rodata*) }
    .data             :  { *(.sdata*) }
    __bss_start       = .;
    .bss              :  { *(.bss*)  *(COMMON) }
    __bss_end         = .;
}

ENTRY(_start)
