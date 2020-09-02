/* 
 * File: nameless.c
 * ----------------
 * This module is compiled WITHOUT the -mpoke-function-name option.
 * This means that the functions in this module do not have 
 * their name stored in the text section.  We refer to these 
 * functions as "nameless". 
 * 
 * A nameless function should be identiied as "???" in the backtrace.
 */

#include "backtrace.h"
#include "nameless.h"
#include "printf.h"

static int magic(int x)
{
    printf("Here is a complex backtrace, including nameless functions:\n");
    print_backtrace();
    return x + 2;
}

int mystery(void)
{
    return magic(1);
}
