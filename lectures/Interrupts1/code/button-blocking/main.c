#include "console.h"
#include "echo.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "uart.h"

static int gCount = 0;

static void update_screen(void) {
    console_clear();
    console_printf("\nCount of button presses\n");
    for (int r = 0; r < 80; r++) {
        console_printf("%4d ", gCount);
    }
    console_printf("\n");
}

static void wait_for_press(gpio_id_t button) {
    while (gpio_read(button) != 1) ; // loop until becomes high
    while (gpio_read(button) == 1) ; // loop while stays high
    gCount++;
}

void main(void) {
    gpio_id_t button = GPIO_PB4;

    uart_init();
    gpio_set_input(button);
    gpio_set_pullup(button);
    console_init(24, 50, GL_AMBER, 0xff222222);

    while (1) {
        console_printf("\nWaiting for button click...\n");
        wait_for_press(button);
        update_screen();
    }
}
