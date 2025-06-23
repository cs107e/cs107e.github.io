#ifndef MEMMAP_H
#define MEMMAP_H

/*
 * Interface to symbols placed by library linker script memmap.ld
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

/*
 * These markers are used to identify the boundaries of the various
 * program sections. These symbols are placed by the linker
 * script memmap.ld
 */

#include <stdint.h>


extern unsigned char __text_end;
extern unsigned char __bss_start, __bss_end;
extern unsigned char __heap_start, __heap_max;
extern unsigned char __stack_top;
extern unsigned char __elf_start;


#warning JDZ TODO memmap symbols
//const uintptr_t TEXT_START = 0x40000000;
// const uintptr_t STACK_TOP = &__stack_top;
// const uintptr_t HEAP_START = &__heap_start;
// const uintptr_t HEAP_MAX = &__heap_max;
// const uintptr_t BSS_START = &__bss_start;
// const uintptr_t BSS_END = &__bss_end;


/*
 * Interface of function symbols used in the start sequence.
 */
extern void _start(void);
extern void _cstart(void);
extern void main(void);

#define TEXT_START_ADDR ((uintptr_t)0x40000000)

#endif
