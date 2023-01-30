#include "uart.h"

void main(void)
{
    uart_init();

    while(1) {        
        unsigned int ch=uart_getchar();
        uart_putchar(ch);
    }
}
