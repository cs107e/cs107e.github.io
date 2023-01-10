#pragma once
#include "gpio_interrupts.h"
void ref_gpio_interrupts_init(void);
void ref_gpio_interrupts_enable(void);
void ref_gpio_interrupts_disable(void);
void ref_gpio_interrupts_register_handler(unsigned int pin, handler_fn_t fn, void *aux_data);
