#include "printf.h"

// Cheezy way to echo console to uart
static int both_printf(const char *format, ...)
{
    char buf[1000];

    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    console_printf(buf);
    return printf(buf);
}

#define console_printf both_printf
