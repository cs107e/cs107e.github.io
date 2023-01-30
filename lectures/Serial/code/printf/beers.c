#include "timer.h"
#include "gpio.h"
#include "printf.h"
#include "uart.h"

void main(void)
{
    uart_init();

    int num = 100;
    while (num >= 0) {
        printf("%d bottles of beer on the wall, %d bottles of beer.\n", 
            num, num);
        num--;
        printf("Take one down and pass it around, %d bottles of beer on the wall.\n", 
            num);
        timer_delay(1);
    }
}
