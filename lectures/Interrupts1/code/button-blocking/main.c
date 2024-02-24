#include "console.h"
#include "echo.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "uart.h"

#define BUTTON GPIO_PG13

static int gCount = 0;

static void update_screen() {
    console_clear();
    console_printf("\nCount of button presses\n");
    for (int r = 0; r < 80; r++) {
        console_printf("%4d ", gCount);
    }
    console_printf("\n");
}

static void wait_for_press(void) {
    while (gpio_read(BUTTON) != 1) ; // loop until becomes high
    while (gpio_read(BUTTON) == 1) ; // loop while stays high
    gCount++;
}

void main(void) {
    uart_init();
    gpio_set_input(BUTTON);
    gpio_set_pullup(BUTTON);
    console_init(24, 50, GL_AMBER, 0xff222222);

    while (1) {
        console_printf("\nWaiting for button click...\n");
        wait_for_press();
        update_screen();
    }
}
