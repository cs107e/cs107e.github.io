#ifndef ASSERT_H
#define ASSERT_H

#include "pi.h"
#include "uart.h"

#define STRINGIFY_IMPL(x) #x
#define AS_STRING(x) STRINGIFY_IMPL(x)

#define assert(EXPR) \
   do { \
        if (!(EXPR)) { \
            uart_putstring("File " __FILE__ ", line " AS_STRING(__LINE__)  ": Assertion '" #EXPR "' failed.\n\04"); \
            pi_abort(); \
        } \
   } while (0);

#endif
