#include "malloc.h"
#include "printf.h"
#include <stdbool.h>
#include "strings.h"
#include "uart.h"

/*
 * Returns a pointer to a new null-terminated string containing at most `n`
 * bytes copied from the string pointed to by `src`.
 *
 * Example: strndup("cs107e", 4) == "cs10"
 */
static char *strndup(const char *src, size_t n)
{
    // TODO fill this in
    return "";
}

static bool isspace(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n';
}

static int tokenize(const char *line, char *array[],  int max)
{
    int ntokens = 0;
    const char *cur = line;

    while (ntokens < max) {
        while (isspace(*cur)) cur++;    // skip spaces (stop non-space/null)
        if (*cur == '\0') break;        // no more non-space chars
        const char *start = cur;
        while (*cur != '\0' && !isspace(*cur)) cur++; // advance to end (stop space/null)
        array[ntokens++] = strndup(start, cur - start);   // make heap-copy, add to array
    }
    return ntokens;
}

void main(void)
{
    uart_init();

    char *str = "Leland Stanford Junior University Established 1891";
    int max = strlen(str); // number of tokens is at most length of string
    
    char *array[max];   // declare stack array to hold strings

    int ntokens = tokenize(str, array, max);
    for (int i = 0; i < ntokens; i++)
        printf("[%d] = %s\n", i, array[i]);

    uart_putchar(EOT);
}
