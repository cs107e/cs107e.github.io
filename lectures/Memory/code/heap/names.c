#include "printf.h"
#include "strings.h"
#include "uart.h"

#define MAX_LINE_LEN 64

static int get_and_echo(void)
{
    char ch = uart_getchar();   // read input from terminal
    return uart_putchar(ch);    // echo char to terminal
}

static char *read_line(void)
{
    char buf[MAX_LINE_LEN];
    char *result = buf; // what memory address does result point to?

    for (int i = 0; i < MAX_LINE_LEN - 1; i++) {
        char ch = get_and_echo();
        result[i] = ch == '\n' ? '\0' : ch;
        if (!result[i]) break;
    }
    return result;
}


void main(void)
{
    uart_init();

    int n = 3;
    char *names[n];

    printf("\nEnter %d names, one per line\n", n);
    for (int i = 0; i < n; i++) {
        printf("Name: ");
        char *line = read_line();
        names[i] = line;
    }
    printf("Read %d names.\n", n);

    printf("\nHere is what I read:\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] = %p \"%s\"\n", i, names[i], names[i]);
    }
    printf("\nProgram completed. ctrl-a k to exit screen.");
    uart_putchar(EOT);
}
