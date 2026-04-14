#include "gpio.h"
#include "timer.h"

const gpio_id_t pin = GPIO_PB8; // uart tx connection

#define BAUD 9600
#define DELAY (1000000/BAUD)

void putbit(int val) {
    gpio_write(pin, val);
    timer_delay_us(DELAY);
}

void putchar(int c) {
    putbit(0);             // start bit
    for (int i = 0; i < 8; i++) {
        putbit(c & 0x1);   // 8 data bits, lsb first
        c >>= 1;
    }
    putbit(1);             // stop bit
    timer_delay_us(2 * DELAY);
}

void puts(char *s) {
    while (*s) {
        putchar(*s++);
    }
}

void main(void) {
    timer_init();
    gpio_init();
    gpio_set_output(pin);

    while (1) {
        puts("hello, laptop\r\n");
        timer_delay(1);
    }
}
