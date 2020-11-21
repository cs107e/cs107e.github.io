#ifndef CONSOLE_REF
#define CONSOLE_REF
#include "console.h"
void ref_console_init(unsigned int nrows, unsigned int ncols);
void ref_console_clear(void);
int ref_console_printf(const char *format, ...) __attribute__((format(printf, 1, 2)));
#endif
