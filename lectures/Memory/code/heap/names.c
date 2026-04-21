#include "printf.h"
#include "read.h"
#include "strings.h"
#include "uart.h"

#define MAX_LINE_LEN 128

static char *read_line(void) {
    char buf[MAX_LINE_LEN];
    char *result = buf; // what memory address does result point to?

    for (int i = 0; i < MAX_LINE_LEN - 1; i++) {
        result[i] = read_char();
        if (!result[i]) break;
    }
    return result;
}

void main(void) {
    uart_init();

    int n = 3;
    char *names[n];

    printf("\nEnter %d names, one per line\n", n);
    for (int i = 0; i < n; i++) {
        printf("Name: ");
        names[i] = read_line();
        printf("Read name '%s'\n", names[i]);
    }

    printf("\nRead %d names. Here they are\n", n);
    for (int i = 0; i < n; i++) {
        printf("[%d] = %p \"%s\"\n", i, names[i], names[i]);
    }
}
