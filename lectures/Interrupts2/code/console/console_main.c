#include "gpio.h"
#include "uart.h"
#include "keyboard.h"
#include "interrupts.h"
#include "console.h"
#include "shell.h"

/* This program tests the keyboard using the console.
 * Console reads from the keyboard and displays on screen.
 * If the keyboard is not using interrupts, it drops events
 * because the screen refresh is so time consuming.
 */

void main(void) {
    gpio_init();
    uart_init();

    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
    keyboard_use_interrupts();

    console_init(40, 80); // calls gl_init
    shell_init(console_printf);

    interrupts_global_enable();
    shell_run();
}
