#include "printf.h"
#include "uart.h"

void interrupt_dispatch(unsigned int pc)
{
}

extern unsigned int _vectors, _vectors_end;
extern unsigned int *_RPI_INTERRUPT_VECTOR_BASE;

void interrupts_init(void)
{
    // copy table of vectors to destination RPI_INTERRUPT_VECTOR_BASE
    // _vector and _vector_end are symbols defined in interrupt_asm.s
    // that mark start/end of table to be copied 
    unsigned int *dst = _RPI_INTERRUPT_VECTOR_BASE;
    unsigned int *src = &_vectors;
    unsigned int n = &_vectors_end - &_vectors;
    for (int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

void main(void) 
{
    interrupts_init();
    printf("Installed interrupt vector table\n");
}

