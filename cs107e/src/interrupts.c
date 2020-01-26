#include "assert.h"
#include "interrupts.h"
#include <stdint.h>

// http://xinu.mscs.mu.edu/BCM2835_Interrupt_Controller
// BCM: p196
// This struct is declared to match memory layout of the interrupt registers
struct interrupt_t {
    uint32_t pending_basic;
    uint32_t pending[2];
    uint32_t fiq_control;
    uint32_t enable[2];
    uint32_t enable_basic;
    uint32_t disable[2];
    uint32_t disable_basic;
};

#define INTERRUPT_CONTROLLER_BASE 0x2000B200

static volatile struct interrupt_t * const interrupt = (struct interrupt_t *)INTERRUPT_CONTROLLER_BASE;

void interrupts_enable_basic(unsigned int n)
{
    if (n > 7) return;

    interrupt->enable_basic |= (1 << n);
}

void interrupts_disable_basic(unsigned int n)
{
    if (n > 7) return;

    interrupt->disable_basic |= (1 << n);
}

void interrupts_enable_source(unsigned int n)
{
    if (n < INTERRUPTS_AUX || n > INTERRUPTS_VC_UART) return;

    unsigned int bank = n / 32;
    unsigned int shift = n % 32;
    interrupt->enable[bank] |= 1 << shift;
}

void interrupts_disable_source(unsigned int n)
{
    if (n < INTERRUPTS_AUX || n > INTERRUPTS_VC_UART) return;

    unsigned int bank = n / 32;
    unsigned int shift = n % 32;
    interrupt->disable[bank] |= 1 << shift;
}

// Check if interrupt vector was installed into correct place
// (supposed to be done in cstart.c). Otherwise there is no
// point in attaching handler (if vector not installed, no
// way to call out)
static bool vector_is_installed(void)
{
    const int IRQ_INDEX = 7;
    extern int _vectors, *_RPI_INTERRUPT_VECTOR_BASE;
    return _RPI_INTERRUPT_VECTOR_BASE[IRQ_INDEX] == (&_vectors)[IRQ_INDEX];
}

#define MAX_HANDLERS 32

static struct isr_t {
    bool (*fn[MAX_HANDLERS])(unsigned int);
    int count;
} handlers;

void interrupts_attach_handler(bool (*isr)(unsigned int))
{
    assert(vector_is_installed());
    assert(handlers.count < MAX_HANDLERS);
    handlers.fn[handlers.count++] = isr;
}

void interrupt_vector(unsigned int pc)
{
    for (int i = 0; i < handlers.count; i++)
        if (handlers.fn[i](pc))     // stop at first handler that claims to handle event
            break;
}
