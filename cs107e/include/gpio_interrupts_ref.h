#ifndef GPIO_INTERRUPTS_REF
#define GPIO_INTERRUPTS_REF
#include "gpio_interrupts.h"
void ref_gpio_interrupts_init(void);
void ref_gpio_interrupts_enable(void);
void ref_gpio_interrupts_disable(void);
handler_fn_t ref_gpio_interrupts_register_handler(unsigned int pin, handler_fn_t fn);
bool ref_gpio_default_handler(unsigned int pc);
#endif
