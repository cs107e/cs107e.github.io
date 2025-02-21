#include "backtrace.h"
#include "console.h"
#include "echo.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "gpio_interrupt.h"
#include "uart.h"

static volatile int gCount = 0;

static void update_screen(void) {
    console_clear();
    console_printf("\nCount of button presses\n");
    for (int r = 0; r < 80; r++) {
        console_printf("%4d ", gCount);
    }
    console_printf("\n");
}

static void button_pressed(void *aux_data) {
    gCount++;
    gpio_id_t button = *(gpio_id_t *)aux_data;
    gpio_interrupt_clear(button);
}

void main(void) {
    gpio_id_t button = GPIO_PB4;
    uart_init();
    gpio_set_input(button);
    gpio_set_pullup(button);

    interrupts_init();
    gpio_interrupt_init();
    gpio_interrupt_config(button, GPIO_INTERRUPT_NEGATIVE_EDGE, true);
    gpio_interrupt_register_handler(button, button_pressed, &button);
    gpio_interrupt_enable(button);
    interrupts_global_enable();

    console_init(24, 50, GL_AMBER, 0xff222222);
    console_printf("\nWaiting for button click...\n");

    int last_count = gCount;
    while (1) {
        if (last_count != gCount) {
            last_count = gCount;
            update_screen();
            console_printf("\nWaiting for button click...\n");
        }
    }
}
