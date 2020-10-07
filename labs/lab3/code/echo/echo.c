#include "uart.h"

void main (void)
{
    uart_init();    // must set up uart peripheral before using, init once

    int ch;

    do {
        ch = uart_getchar();
        uart_putchar(ch);
    } while (ch != EOF);

    uart_putchar(EOT); // not strictly necessary, but signals that program is done
}
