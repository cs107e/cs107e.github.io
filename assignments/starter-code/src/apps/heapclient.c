#include "backtrace.h"
#include "malloc.h"
#include "printf.h"
#include <stdbool.h>
#include "strings.h"
#include "uart.h"


static char *strndup(const char *src, size_t n)
{
    static bool been_here = false;
    if (!been_here) {
        print_backtrace();  // only print backtrace on first call to strndup
        been_here = true;
    }

    char *copy = malloc(n+1);
    copy[n] = '\0';
    return memcpy(copy, src, n);
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

static char *reverse_concat(char *tokens[],  int n)
{
    char *result = strndup("", 0);

    for (int i = n-1; i >= 0; i--) {
        size_t nbytes = strlen(result) + strlen(tokens[i]) + 2; // extra 1 for space char between tokens
        result = realloc(result, nbytes); // exercise realloc, too!
        strlcat(result, tokens[i], nbytes);
        strlcat(result, " " , nbytes);
    }
    return result;
}

static void heap_practice(const char *str)
{
    int max = strlen(str); // number of tokens is at most length of string

    char *array[max];   // declare stack array to hold strings

    printf("\nTokenizing:\n%s\n", str);
    int ntokens = tokenize(str, array, max);
    for (int i = 0; i < ntokens; i++) {
        printf("[%d] = %s\n", i, array[i]);
    }

    char *reversed = reverse_concat(array, ntokens);
    printf("Tokens concatenated in reverse order:\n%s\n", reversed);
    free(reversed);
    for (int i = 0; i < ntokens; i++) {
        free(array[i]);
    }
}


void main(void)
{
    uart_init();

    heap_practice("Leland Stanford Junior University Established 1891");
    heap_practice("the quick brown fox jumped over the lazy spotted dog");
    heap_practice("We <3 printf!");

    uart_putchar(EOT);
}
