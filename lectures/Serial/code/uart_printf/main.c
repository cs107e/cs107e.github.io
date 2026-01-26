#include "printf.h"
#include "timer.h"
#include "uart.h"

void echo(void) {
    int ch;

    printf("Type characters (end with q): ");
    do {
        ch = uart_getchar(); // read char laptop TX -> mango RX
        uart_putchar(ch);        // write char mango TX -> laptop RX
    } while (ch != 'q');
}

void main(void) {
    uart_init();

    echo();

    for (int num = 100; num > 0; num--) {
        printf("\n%d bottles of beer on the wall, %d bottles of beer.\n",
            num, num);
        printf("Take one down and pass it around, %d bottles of beer on the wall.\n",
            num - 1);
        timer_delay(1);
    }
}
