#ifndef INTERRUPTS_REF
#define INTERRUPTS_REF
#include "interrupts.h"
void ref_interrupts_init(void);
void ref_interrupts_global_enable(void);
void ref_interrupts_global_disable(void);
bool ref_interrupts_is_pending(unsigned int source);
bool ref_interrupts_enable_source(unsigned int source);
bool ref_interrupts_disable_source(unsigned int source);
handler_fn_t ref_interrupts_register_handler(unsigned int source, handler_fn_t fn);
#endif
