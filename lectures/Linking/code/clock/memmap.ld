SECTIONS
{
    .text 0x40000000      : { *(.text.start) *(.text*) }
    .rodata               : { *(.rodata*) *(.srodata*) }
    .data  ALIGN(0x20000) : { *(.sdata*) }
    __bss_start           = .;
    .bss                  : { *(.bss*)  *(.sbss*) *(COMMON) }
    __bss_end             = .;
}
