#ifndef MEMMAP_H
#define MEMMAP_H

/*
 * Interface to symbols placed by library linker script memmap.ld
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdint.h>

/*
 * These symbols named with double underscores identify the
 * boundaries of the various program sections. These markers are
 * placed by the linker script memmap.ld. Your code should not
 * directly access these marker symbols; instead use the constants
 * below.
 */

extern unsigned char __text_end;
extern unsigned char __bss_start, __bss_end;
extern unsigned char __heap_start, __heap_max;
extern unsigned char __stack_top;
extern unsigned char __elf_start;

/*
 * The public constants below provide access to the addresses
 * marked in the memmap.ld linker script.  Each constant is
 * the address of its corresponding marker symbol, e.g.
 * TEXT_END_ADDR for &__text_end or HEAP_START for _&_heap_start.
 * Use these constants when you need to access the memory
 * locations marked by the linker script.
 */

#define PROGRAM_LOAD_ADDR 0x40000000

static uintptr_t const TEXT_START_ADDR = (uintptr_t)PROGRAM_LOAD_ADDR;
static uintptr_t const TEXT_END_ADDR   = (uintptr_t)&__text_end;

static uint8_t * const BSS_START       = &__bss_start;
static uint8_t * const BSS_END         = &__bss_end;

static void * const HEAP_START         = &__heap_start;
static void * const HEAP_MAX           = &__heap_max;

static void * const STACK_TOP          = &__stack_top;

static void * const ELF_START          = &__elf_start;

/*
 * Interface of function symbols used in the start sequence.
 */
extern void _start(void);
extern void _cstart(void);
extern void main(void);

#endif