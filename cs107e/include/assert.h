#ifndef ASSERT_H
#define ASSERT_H

#include "pi.h"
#include "uart.h"

#define STRINGIFY_IMPL(x) #x
#define AS_STRING(x) STRINGIFY_IMPL(x)

#define assert(EXPR) \
   do { \
        if (!(EXPR)) { \
            uart_putstring("File " __FILE__ ", line " AS_STRING(__LINE__) ", function "); \
            uart_putstring(__func__); \
            uart_putstring("() : Assertion '" #EXPR "' failed.\n"); \
            uart_putchar(TERMINATE); \
            uart_putchar(EOT); \
            pi_abort(); \
        } \
   } while (0);

#endif


// latest version of rpi-run.py (v2.3) now recognizes/reports TERMINATE control code
// older version only recognizes EOT
// output both so EOT acts as backstop if using older rpi-run.py
// (could eventually remove when everyone up to date)