#pragma once
#include "console.h"
void ref_console_init(unsigned int nrows, unsigned int ncols, color_t foreground, color_t background);
void ref_console_clear(void);
int ref_console_printf(const char *format, ...) __attribute__((format(printf, 1, 2)));
