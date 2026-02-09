#include "gpio.h"
#include "gpio_extra.h"
#include "keyboard.h"
#include "printf.h"
#include "uart.h"

static void wait_for_falling_clock_edge(gpio_id_t clock) {
    while (gpio_read(clock) != 1) {} // wait until clock is high
    while (gpio_read(clock) == 1) {} // wait until clock is low
}

static int read_bit(gpio_id_t clock, gpio_id_t data) {
    wait_for_falling_clock_edge(clock);
    return gpio_read(data);
}

void main(void) {
    gpio_init();
    uart_init();

    gpio_id_t clock = KEYBOARD_CLOCK; // which gpios are these? read keyboard.h to find out!
    gpio_id_t data = KEYBOARD_DATA;

    gpio_set_input(clock);
    gpio_set_pullup(clock); // set_pullup in gpio_extra.h

    gpio_set_input(data);
    gpio_set_pullup(data);

    for (int i = 0; /* forever */; i++) {
        printf("[%d]: ", i);
        for (int i = 0; i < 11; i++) {
            int bit = read_bit(clock, data);
            uart_putchar(bit + '0');
        }
        uart_putchar('\n');
    }
}
