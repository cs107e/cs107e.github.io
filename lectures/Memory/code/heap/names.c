#include "printf.h"
#include "strings.h"
#include "uart.h"

static void swap(char **p, char **q)
{
    char *tmp = *p;
    *p = *q;
    *q = tmp;
}

static void sort(char *arr[], int n)
{
    for (int i = 0; i < n; i++) {
        int min = i;
        for (int j = i; j < n; j++) {
            if (strcmp(arr[j], arr[min]) < 0) min = j;
        }
        swap(&arr[i], &arr[min]);
    }
}

static int get_and_echo(void)
{        
    char ch = uart_getchar();   // read input from terminal
    uart_putchar(ch);           // echo char to terminal
    return ch;
}

static char *read_line(void)
{
    size_t buflen = 20;
    char buf[buflen];

    for (int i = 0; i < buflen - 1; i++) {
        char ch = get_and_echo();
        buf[i] = ch == '\n' ? '\0' : ch;
        if (!buf[i]) break;
    }
    return buf; // compiler warning here, what's wrong?
}


void main(void)
{
    uart_init();

    char *names[10];
    int n;

    while (1) {
        printf("\nEnter up to 10 names, one per line, END to quit\n");
        for (n = 0; n < 10; n++) {
            char *line = read_line();
            if (strcmp(line, "END") == 0) break;
            names[n] = line;
        }
        if (n == 0) break;
        printf("Read %d names.\n", n);


        printf("\nNow in sorted order:\n");
        sort(names, n);
        for (int i = 0; i < n; i++) {
            printf("[%d] = %p %s\n", i, names[i], names[i]);
        }
    }

    uart_putchar(EOT);
}
