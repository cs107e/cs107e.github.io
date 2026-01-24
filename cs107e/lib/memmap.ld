STACK_SIZE = 0x100000; /* 1 MB */

SECTIONS
{
    .text  0x40000000     :  { *(.text.start) *(.text*) }
    __text_end            = .;
    .rodata               :  { *(.rodata*) *(.srodata*) }
    .data  ALIGN(0x20000) :  { *(.sdata*) }
    __bss_start           = .;
    .bss                  :  { *(.bss*)  *(.sbss*) *(COMMON) }
    __bss_end             = .;
    .heap  0x44000000     :  { __heap_start = .; }
    .stack 0x50000000 - STACK_SIZE : {
        __heap_max = .;
        . = . + STACK_SIZE;
        __stack_top = .;
    }
    __elf_start = .;
}

ASSERT(_start == ADDR(.text),  /* error if _start not at correct address */
       "_start symbol must be placed first in text section")
ENTRY(_start_gdb)              /* set elf entry point (used by gdb) */
