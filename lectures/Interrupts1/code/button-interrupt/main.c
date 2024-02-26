#include "backtrace.h"
#include "console.h"
#include "echo.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "gpio_interrupt.h"
#include "uart.h"

#define BUTTON GPIO_PG13

static volatile int gCount = 0;

static void update_screen() {
    console_clear();
    console_printf("\nCount of button presses\n");
    for (int r = 0; r < 80; r++) {
        console_printf("%4d ", gCount);
    }
    console_printf("\n");
}

static void button_pressed(uintptr_t pc, void *aux_data) {
    gCount++;
    gpio_interrupt_clear(BUTTON);
}

void main(void) {
    uart_init();
    gpio_set_input(BUTTON);
    gpio_set_pullup(BUTTON);

    interrupts_init();
    gpio_interrupt_init();
    gpio_interrupt_config(BUTTON, GPIO_INTERRUPT_NEGATIVE_EDGE, false);
    gpio_interrupt_register_handler(BUTTON, button_pressed, NULL);
    gpio_interrupt_enable(BUTTON);
    interrupts_global_enable();

    console_init(24, 50, GL_AMBER, 0xff222222);
    console_printf("\nWaiting for button click...\n");

    int last_count = gCount;
    while (1) {
        if (last_count != gCount) {
            update_screen();
            last_count = gCount;
            console_printf("\nWaiting for button click...\n");
        }
    }
}
