#include "gpio.h"
#include "gpio_extra.h"
#include "uart.h"

static const gpio_id_t CLK = GPIO_PG12;
static const gpio_id_t DATA = GPIO_PB7;

static void wait_for_falling_clock_edge(void) {
    while (gpio_read(CLK) == 0) {}
    while (gpio_read(CLK) == 1) {}
}

static int read_bit(void) {
    wait_for_falling_clock_edge();
    // read DATA when clock is low
    return gpio_read(DATA);
}

void main(void) {
    gpio_init();
    uart_init();

    gpio_set_input(CLK);
    gpio_set_pullup(CLK); // From gpio_extra.h

    gpio_set_input(DATA);
    gpio_set_pullup(DATA);

    while (1) {
        for (int i = 0; i < 11; i++) {
            int bit = read_bit();
            uart_putchar(bit + '0');
        }
        uart_putchar('\n');
    }
}
