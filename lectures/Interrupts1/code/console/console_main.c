#include "console.h"
#include "gpio.h"
#include "interrupts.h"
#include "keyboard.h"
#include "shell.h"
#include "uart.h"

/* This program tests the keyboard using the console.
 * Console reads from the keyboard and displays on screen.
 * If the keyboard is not using interrupts, it drops events
 * because the screen refresh is so time consuming.
 */

void main(void) 
{
    gpio_init();
    uart_init();
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
    console_init(30, 50);
    shell_init(console_printf);

    // interrupts_init();
    // keyboard_use_interrupts();
    // interrupts_global_enable();

    shell_run();
}
