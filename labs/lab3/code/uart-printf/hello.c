#include "gpio.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

volatile unsigned int *FSEL2 = (unsigned int *)0x20200008;

void main (void)
{
    uart_init();    // must set up uart peripheral before using, init once

    for (int i = 0; i < 5; i++) {
        uart_putstring("hello, laptop\n");
        timer_delay(1);
    }
    printf("We %s printf!\n", "<3");
    
    uart_putchar(EOT); // not strictly necessary, but signals to rpi-install that program is done
}
