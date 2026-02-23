#include "console.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "gpio_interrupt.h"
#include "hstimer.h"
#include "interrupts.h"
#include "keyboard.h"
#include "printf.h"
#include "uart.h"
#include "timer.h"

#define BUTTON GPIO_PB4

static volatile int elapsed, last_button, last_scan, nbutton, nscan;

static void button_click(void *aux_data) {
    gpio_interrupt_clear(BUTTON);
    last_button = elapsed;
    nbutton++;
}

static void second_elapsed(void *aux_data) {
    hstimer_interrupt_clear(HSTIMER0);
    elapsed++;
}

static void clock_edge(void *aux_data) {
    static int nClock; // count clock edges, 11 edges = one scancode

    gpio_interrupt_clear(KEYBOARD_CLOCK);
    nClock++;
    if (nClock % 11 == 0) {
        last_scan = elapsed;
        nscan++;
    }
}

void configure_button(void) {
    gpio_interrupt_init();
    gpio_set_input(BUTTON);
    gpio_set_pullup(BUTTON);
    gpio_interrupt_config(BUTTON, GPIO_INTERRUPT_NEGATIVE_EDGE, true); // debounce = true
    gpio_interrupt_register_handler(BUTTON, button_click, NULL);
    gpio_interrupt_enable(BUTTON);
}

void configure_alarm(void) {
    // countdown expressed in number of usecs
    hstimer_init(HSTIMER0, 1000000);
    hstimer_enable(HSTIMER0);             // start timer
    interrupts_register_handler(INTERRUPT_SOURCE_HSTIMER0, second_elapsed, NULL);
    interrupts_enable_source(INTERRUPT_SOURCE_HSTIMER0);
}

void configure_keyboard(void) {
    gpio_interrupt_init();
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
    gpio_interrupt_config(KEYBOARD_CLOCK, GPIO_INTERRUPT_NEGATIVE_EDGE, false); // no debounce!
    gpio_interrupt_register_handler(KEYBOARD_CLOCK, clock_edge, NULL);
    gpio_interrupt_enable(KEYBOARD_CLOCK);
}

void main(void) {
    uart_init();
    interrupts_init();

    configure_button();
    configure_alarm();
    configure_keyboard();

    interrupts_global_enable();

    console_init(12, 20, GL_INDIGO, GL_WHITE);

    while (1) {
        console_printf("\f\n  Time now %02d:%02d\n\n"
                       "\n scancode count %d"
                       "\n   last @t %02d:%02d\n"
                       "\n button count %d"
                       "\n   last @t %02d:%02d",
                        elapsed/60, elapsed % 60,
                        nscan, last_scan/60, last_scan % 60,
                        nbutton, last_button/60, last_button % 60);
        timer_delay(1);
    }
}
