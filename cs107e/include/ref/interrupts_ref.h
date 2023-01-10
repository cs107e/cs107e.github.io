#pragma once
#include "interrupts.h"
void ref_interrupts_init(void);
void ref_interrupts_global_enable(void);
void ref_interrupts_global_disable(void);
void ref_interrupts_enable_source(unsigned int source);
void ref_interrupts_disable_source(unsigned int source);
void ref_interrupts_register_handler(unsigned int source, handler_fn_t fn, void *aux_data);
