#include "malloc.h"
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
    char ch = uart_getchar();
    if (ch == '\r') ch = '\n';
    uart_putchar(ch);
    return ch;
}

static char *read_line(void)
{
    size_t buflen = 128;
    char buf[buflen];
    int n;

    for (n = 0; n < buflen - 1; n++) {
        char ch = get_and_echo();
        buf[n] = (ch == '\n') ? '\0' : ch;
        if (!buf[n]) break;
    }
    return buf;
}


void main(void)
{
    uart_init();

    char *names[10];
    int n;

    printf("Enter names, one per line, END to quit\n");
    for (n = 0; n < 10; n++) {
        char *line = read_line();
        if (strcmp(line, "END") == 0) break;
        names[n] = line;
    }

    sort(names, n);

    printf("Read %d names.\n\nNow in sorted order:\n", n);
    for (int i = 0; i < n; i++)
        printf("%s\n", names[i]);
    printf("\04");
}
