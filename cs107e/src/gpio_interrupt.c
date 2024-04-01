/*
 * GPIO interrupt handling
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Updated Thu Feb 15 12:28:12 PST 2024
 */

#include "gpio_interrupt.h"
#include "_gpio_common.h"
#include "assert.h"
#include "gpio_extra.h"
#include "interrupts.h"
#include <stddef.h>

typedef union {
    struct {
        uint32_t cfg[4]; // only 1-2 in use
        uint32_t ctl;
        uint32_t status;
        uint32_t debounce;
    } regs;
    unsigned char padding[0x20];
} gpio_eint_t;

#define GPIO_EINT_BASE       ((gpio_eint_t *)0x2000220)
_Static_assert(&(GPIO_EINT_BASE[GROUP_C].regs.cfg[0])   == (uint32_t *)0x2000240, "PC irq cfg0 reg must be at address 0x2000240");
_Static_assert(&(GPIO_EINT_BASE[GROUP_E].regs.debounce) == (uint32_t *)0x2000298, "PE irq debounce reg must be at address 0x2000298");

typedef struct {
    volatile gpio_eint_t *const eint;
    const int max_pin_index;
    const interrupt_source_t source;
    struct {
        handlerfn_t fn;
        void *aux_data;
    } handlers[GPIO_MAX_PIN_INDEX];
} gpio_int_group_t;

static struct {
    gpio_int_group_t groups[GPIO_NGROUPS];
    bool initialized;
} module = {
    .groups = { {.eint= GPIO_EINT_BASE + GROUP_B, .max_pin_index= GPIO_PB_LAST_INDEX, .source= INTERRUPT_SOURCE_GPIOB},
                {.eint= GPIO_EINT_BASE + GROUP_C, .max_pin_index= GPIO_PC_LAST_INDEX, .source= INTERRUPT_SOURCE_GPIOC},
                {.eint= GPIO_EINT_BASE + GROUP_D, .max_pin_index= GPIO_PD_LAST_INDEX, .source= INTERRUPT_SOURCE_GPIOD},
                {.eint= GPIO_EINT_BASE + GROUP_E, .max_pin_index= GPIO_PE_LAST_INDEX, .source= INTERRUPT_SOURCE_GPIOE},
                {.eint= GPIO_EINT_BASE + GROUP_F, .max_pin_index= GPIO_PF_LAST_INDEX, .source= INTERRUPT_SOURCE_GPIOF},
                {.eint= GPIO_EINT_BASE + GROUP_G, .max_pin_index= GPIO_PG_LAST_INDEX, .source= INTERRUPT_SOURCE_GPIOG},
              },
    .initialized = false,
};

static void dispatch_to_pin(void *group_num);

static gpio_int_group_t *get_int_group(gpio_id_t gpio, int *p_index) {
    gpio_pin_t p = get_group_and_index(gpio);
    *p_index = p.pin_index;
    return &module.groups[p.group];
}

// register dispatch_to_pin with top-level interrupts module
// as handler for all GPIO interrupt sources
// Aux data will be pointer to interrupt group
void gpio_interrupt_init(void) {
    for (int i = 0; i < GPIO_NGROUPS; i++) {
        interrupts_register_handler(module.groups[i].source, dispatch_to_pin, &module.groups[i]);
        interrupts_enable_source(module.groups[i].source);
    }
    module.initialized = true;
}

void gpio_interrupt_register_handler(gpio_id_t gpio, handlerfn_t fn, void *aux_data) {
    if (!module.initialized) error("gpio_interrupt_init() has not been called!\n");
    assert(gpio_id_is_valid(gpio));
    int pin_index;
    gpio_int_group_t *gp = get_int_group(gpio, &pin_index);
    gp->handlers[pin_index].fn = fn;
    gp->handlers[pin_index].aux_data = aux_data;
}

// dispatch_to_pin handler receives all GPIO interrupts and performs second-level dispatch to
// per-pin handlers that have been registered with this module
static void dispatch_to_pin(void *aux_data) {
    gpio_int_group_t *gp = aux_data;
    int pin_index = 31 - __builtin_clz(gp->eint->regs.status);
    gp->handlers[pin_index].fn(gp->handlers[pin_index].aux_data);
}

static void gpio_interrupt_set_enabled(gpio_id_t gpio, bool state) {
 if (!module.initialized) error("gpio_interrupt_init() has not been called!\n");
    assert(gpio_id_is_valid(gpio));
    int pin_index;
    gpio_int_group_t *gp = get_int_group(gpio, &pin_index);
    unsigned int mask = (1 << pin_index);
    if (state) {
        gp->eint->regs.ctl |= mask;
    } else {
        gp->eint->regs.ctl &= ~mask;
    }
}

void gpio_interrupt_enable(gpio_id_t gpio) {
    gpio_interrupt_set_enabled(gpio, true);
}

void gpio_interrupt_disable(gpio_id_t gpio) {
    gpio_interrupt_set_enabled(gpio, false);
}

void gpio_interrupt_clear(gpio_id_t gpio) {
    if (!module.initialized) error("gpio_interrupt_init() has not been called!\n");
    assert(gpio_id_is_valid(gpio));
    int pin_index;
    gpio_int_group_t *gp = get_int_group(gpio, &pin_index);
    unsigned int mask = (1 << pin_index);
    if ((gp->eint->regs.status & mask) != 0) { // is pending
        gp->eint->regs.status |= mask; // write 1 to clear
    }
}

void gpio_interrupt_config(gpio_id_t gpio, gpio_event_t event, bool debounce) {
    if (!module.initialized) error("gpio_interrupt_init() has not been called!\n");
    assert(gpio_id_is_valid(gpio) && event <= GPIO_INTERRUPT_DOUBLE_EDGE);
    int pin_index;
    gpio_int_group_t *gp = get_int_group(gpio, &pin_index);
    int bank =  pin_index / 8;
    int index = pin_index % 8;
    int shift = index * 4;
    unsigned int mask = ((1 << 4) - 1);
    gp->eint->regs.cfg[bank] = (gp->eint->regs.cfg[bank] & ~(mask << shift)) | ((event & mask) << shift);
    gpio_set_function(gpio, GPIO_FN_INTERRUPT); // change pin function to interrupt
    if (debounce) {
        // 32Khz clock, predivide 2^5, will filter to ~1 event per ms
        gp->eint->regs.debounce = (5 << 4) | 0;
    } else {
        // 24Mhz clock, no predivide, no filter
        gp->eint->regs.debounce = (0 << 4) | 1;
    }
    gpio_interrupt_clear(gpio); // cancel any stale event
}
