#ifndef ASSERT_H
#define ASSERT_H

/*
 * Interface for assertions.  The `assert` macro tests the given expression and
 * if it is false, prints a diagnostic message and halts execution.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "mango.h"
#include "printf.h"
#include "uart.h"

#define STRINGIFY_IMPL(x) #x
#define AS_STRING(x) STRINGIFY_IMPL(x)

#define assert(EXPR) \
    do { \
        if (!(EXPR)) { \
            uart_putstring("\033[31;1m"); \
            uart_putstring("File " __FILE__ ", line " AS_STRING(__LINE__) ", in function "); \
            uart_putstring(__func__); \
            uart_putstring("() : Assertion '" #EXPR "' failed.\n"); \
            uart_putstring("\033[0m\n"); \
            mango_abort(); \
        } \
    } while (0);

#define error(...) \
    do { \
        uart_putstring("\033[31;1m"); \
        uart_putstring("File " __FILE__ ", line " AS_STRING(__LINE__) ", in function "); \
        uart_putstring(__func__); \
        uart_putstring("()\nERROR: "); \
        printf(__VA_ARGS__); \
        uart_putstring("\033[0m\n"); \
        mango_abort(); \
    } while (0);

#endif
