STACK_SIZE = 0x100000; /* 1 MB */

SECTIONS
{
    .text  0x40000000     :  { *(.text.start) *(.text*) __text_end = .; }
    .rodata               :  { __rodata_start = .; *(.rodata*) *(.srodata*) __rodata_end = .; }
    .data  ALIGN(0x20000) :  { __data_start = .;   *(.sdata*) __data_end = .; }
    .bss                  :  { __bss_start = .; *(.bss*)  *(.sbss*) *(COMMON)  __bss_end = .; }
    .heap  0x44000000     :  { __heap_start = .; }
    .stack 0x50000000 - STACK_SIZE : {
        __heap_max = .;
        . = . + STACK_SIZE;
        __stack_top = .;
    }
}

PROVIDE(_start_gdb = _start);  /* iff no symbol for _start_gdb, provide as alias to _start */
ENTRY(_start_gdb)              /* control entry point, ensure _start symbol is linked */
ASSERT(_start == ADDR(.text),  /* error if _start not at correct address */
       "_start symbol must be placed first in text section")

/* Note: this memmap.ld includes extra symbols used by gdb diagram script */
