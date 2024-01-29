#include "timer.h"
#include "uart.h"

void puts(char *s) {
    while(*s)
        uart_putchar(*s++);
}

void main (void) {
    uart_init();
    while(1) {
        puts("hello, laptop\n");
        timer_delay(1);
    }
}
