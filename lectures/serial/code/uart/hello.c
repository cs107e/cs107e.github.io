#include "timer.h"
#include "uart.h"

void puts(char *s)
{
    while(*s)
        uart_putc(*s++);
}

void notmain (void)
{
    uart_init();
    while(1) {
        puts( "Hello, laptop\n" );
        delay(1);
    }
}
