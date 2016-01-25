#include "timer.h"
#include "uart.h"
#include <stdio.h>

void notmain (void)
{
    uart_init();
    while(1) {
        printf( "Hello, laptop\n" );
        delay(1);
    }
}
