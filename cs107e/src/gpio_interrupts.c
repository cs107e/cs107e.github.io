/* File: gpio_interrupts.c
 * -----------------------
 * Dispatcher for GPIO interrupts.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 *         Julie Zelenski <zelenski@cs.stanford.edu>
 *
 * Last modified: Feb 2022
 */

#include "assert.h"
#include "bits.h"
#include "gpio_interrupts.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "_gpio_private.h"
#include "interrupts.h"
#include <stddef.h>
#include <stdbool.h>

static bool gpio_interrupts_initialized = false;

// This array stores the handlers for gpio interrupts, one per pin.
// If no handler is registered for a given pin, fn at that index is NULL
// global array in bss, values zero'ed at program start
static struct {
    handler_fn_t fn;
    void *aux_data;
} handlers[GPIO_PIN_LAST + 1];

static void gpio_interrupt_dispatch(unsigned int, void *);

void gpio_interrupts_init(void) {
    // okay to re-init this module
    // will not wipe per-pin handlers that were previously registered
    // re-register gpio dispatch as handler with top-level interrupts modules
    // reset gpio interrupt source to disabled state
    interrupts_disable_source(INTERRUPTS_GPIO3);
    interrupts_register_handler(INTERRUPTS_GPIO3, gpio_interrupt_dispatch, NULL);
    gpio_interrupts_initialized = true;
}

void gpio_interrupts_enable(void) {
    assert(gpio_interrupts_initialized);
    interrupts_enable_source(INTERRUPTS_GPIO3);
}

void gpio_interrupts_disable(void) {
    assert(gpio_interrupts_initialized);
    interrupts_disable_source(INTERRUPTS_GPIO3);
}

void gpio_interrupts_register_handler(unsigned int pin, handler_fn_t fn, void *aux_data) {
    assert(gpio_interrupts_initialized);
    assert(pin <= GPIO_PIN_LAST);
    handlers[pin].fn = fn;
    handlers[pin].aux_data = aux_data;
}

static unsigned int get_next_pin(void) {
    unsigned int bank0_zeroes = count_leading_zeroes(_gpio->eds[0]);
    unsigned int bank1_zeroes = count_leading_zeroes(_gpio->eds[1]);
    if (bank0_zeroes != 32) {
        return 31 - bank0_zeroes;
    } else if (bank1_zeroes != 32) {
        return 63 - bank1_zeroes;
    } else {
        return GPIO_PIN_LAST + 1;
    }
}

// gpio_interrupt_dispatch is registered with top-level interrupts
// module as handler for source INTERRUPTS_GPIO3. This handler is
// thus called for all GPIO interrupts and then performs second-level
// dispatch to the per-pin handlers that have been registered with
// this module (gpio_interrupts)
static void gpio_interrupt_dispatch(unsigned int pc, void *unused) {
    unsigned int pin = get_next_pin();
    if (pin <= GPIO_PIN_LAST && handlers[pin].fn != NULL) {
        handlers[pin].fn(pc, handlers[pin].aux_data);
    }
}
