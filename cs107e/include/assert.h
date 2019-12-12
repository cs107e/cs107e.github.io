#ifndef ASSERT_H
#define ASSERT_H

#include "pi.h"
#include "printf.h"

#define assert(EXPR) \
   do { \
        if (!(EXPR)) { \
            printf("File %s, line %d: Assertion '%s' failed.\n\04", __FILE__, __LINE__, #EXPR); \
            pi_abort(); \
        } \
   } while (0);

#endif
