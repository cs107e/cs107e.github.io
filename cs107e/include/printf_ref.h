#ifndef PRINTF_REF
#define PRINTF_REF
#include "printf.h"
int ref_vsnprintf(char *buf, size_t bufsize, const char *format, va_list args);
int ref_snprintf(char *buf, size_t bufsize, const char *format, ...) __attribute__((format(printf, 3, 4)));
int ref_printf(const char *format, ...) __attribute__((format(printf, 1, 2)));
#endif
