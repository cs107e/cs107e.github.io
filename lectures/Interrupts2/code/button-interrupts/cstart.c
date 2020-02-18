/*
 * Reference full implementation of C start sequence for CS107E. This
 * function is called from start.s. _cstart() zeroes out the BSS
 * (assignment 4) and installs interrupt vectors (assignment 7). If main()
 * returns, it turns on the green ACT LED on the Raspberry Pi board.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 *
 * Last update: 2/20/19
 */

// linker memmap places bss symbols at start/end of bss
extern int __bss_start__, __bss_end__;

// _vector and _vector_end are symbols defined in the interrupt
// assembly file, at the beginning and end of the vector and its embedded constants
// RPI_INTERRUPT_VECTOR_BASE is the destination address to copy vector table
extern int _vectors, _vectors_end;
extern int *_RPI_INTERRUPT_VECTOR_BASE;

extern void main(void);

// The C function _cstart is called from the assembly in start.s

void _cstart() {
    int *bss = &__bss_start__;
    int *bss_end = &__bss_end__;

    while (bss < bss_end) { // Zero out the BSS section
        *bss++ = 0;
    }
    
    // Copy interrupt vector table to proper location
    int *vectorsdst = _RPI_INTERRUPT_VECTOR_BASE;
    int *vectors = &_vectors;
    int *vectors_end = &_vectors_end;
    while (vectors < vectors_end) {
        *vectorsdst++ = *vectors++;
    }

    main();

    // After successful completion of main, turn on the green ACT LED (GPIO 47)
    volatile unsigned int *GPIO_FSEL4  = (unsigned int *)0x20200010;
    volatile unsigned int *GPIO_SET1   = (unsigned int *)0x20200020;
    *GPIO_FSEL4 = (*GPIO_FSEL4 & ~(7 << 21)) | (1 << 21);
    *GPIO_SET1 = 1 << 15;
}
