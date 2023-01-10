#include "printf.h"
#include "uart.h"

void main(void) 
{
    uart_init();
    printf("Hello, world!\n");
    uart_putchar(EOT);
}