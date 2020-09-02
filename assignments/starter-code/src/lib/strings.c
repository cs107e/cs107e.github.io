#include "strings.h"

void *memset(void *s, int c, size_t n)
{
    /* TODO: Your code here */
    return NULL;
}

void *memcpy(void *dst, const void *src, size_t n)
{
    /* TODO: Your code here */
    return NULL;
}

size_t strlen(const char *s)
{
    /* Implementation a gift to you from lab3 */
    int n = 0;
    while (s[n] != '\0') {
        n++;
    }
    return n;
}

int strcmp(const char *s1, const char *s2)
{
    /* TODO: Your code here */
    return 0;
}

size_t strlcat(char *dst, const char *src, size_t maxsize)
{
    /* TODO: Your code here */
    return 0;
}

unsigned int strtonum(const char *str, const char **endptr)
{
    /* TODO: Your code here */
    return 0;
}
