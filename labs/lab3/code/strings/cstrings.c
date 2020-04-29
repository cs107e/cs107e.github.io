#include "assert.h"
#include <stddef.h>
#include "uart.h"


// --------  functions to implement -------------------


static size_t strlen(const char *str)
{
    int n = 0;
    while (str[n] != '\0') {
        n++;
    }
    return n;
}

static char *strcpy(char *dst, const char *src)
{
    // Your turn -- implement strcpy!
    return dst;
}


// --------  unit tests from here down -------------------

void test_strlen(void)
{
    char *fruit = "watermelon";

    assert(strlen("green") == 5);
    assert(strlen("") ==  0);
    assert(strlen(fruit) == 2 + strlen(fruit + 2));
}

void test_strcpy(const char *orig)
{
    int len = strlen(orig);
    char buf[len + 1]; // plus one for terminator

    char *copy = strcpy(buf, orig);  
    for (int i = 0; i <= len; i++) // compare letter by letter
        assert(copy[i] == orig[i]);
}

int bogus_strlen_calls(const char *s)
{
    char uninitialized[10];
    char ch = 'A';
    int total = strlen(s);

    // what "should" happen on these calls? what *does* happen?
    total += strlen(uninitialized); // bogus #1
    total += strlen(NULL);          // bogus #2
    total += strlen(&ch);           // bogus #3
    return total;
}

void stress_test_strlen(void)
{
    assert(bogus_strlen_calls("purple") == 7);
}
  
int sketchy_strcpy_call(const char *orig)
{
    // what "should" happen if orig is longer than 8? what *does* happen?
    char copy[8];
    strcpy(copy, orig);
    return strlen(copy);
}

void stress_test_strcpy(void)
{
    assert(sketchy_strcpy_call("hare") == 4);       //  ok
    assert(sketchy_strcpy_call("tortoise") == 8);   // not quite ok
    assert(sketchy_strcpy_call("John Jacob Jingleheimer Schmidt") == 31); // very much not ok
}

void main(void)
{
    uart_init();

    test_strlen();
    //test_strcpy("CS107e rocks");
    //stress_test_strlen();
    //stress_test_strcpy();
    
    uart_putchar(EOT);
}
