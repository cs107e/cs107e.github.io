#include "uart.h"

//  declutter lecture example by moving this function aside

static int read_char(void) {
    char ch = uart_getchar();   // read input from terminal
    uart_putchar(ch);    // echo char to terminal
    return (ch == '\n') ? '\0' : ch; // when read newline, terminate string
}
