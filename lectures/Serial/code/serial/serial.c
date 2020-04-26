#include "timer.h"
#include "gpio.h"

const int pin = GPIO_PIN20;

#define BAUD 9600
#define DELAY (1000000/BAUD)

void putbit(int val)
{
    gpio_write(pin, val);
    timer_delay_us(DELAY);
}

void putchar(int c)
{
    putbit(0); // start bit
    // output 8-bits, lsb first
    for ( int i = 0; i < 8; i++ ) { 
        putbit(c & 0x1);
        c >>= 1;
    }
    putbit(1); // stop bit
    timer_delay_us(2 * DELAY);
}

void puts(char *s)
{
    while (*s)
        putchar(*s++);
}

void main(void)
{
    timer_init();
    gpio_init();

    gpio_set_function(pin, GPIO_FUNC_OUTPUT);
    while (1) {
        puts("hello, laptop\n\r");
        timer_delay_us(8 * DELAY);
    }
}

