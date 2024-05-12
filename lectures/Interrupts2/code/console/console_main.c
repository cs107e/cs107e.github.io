#include "console.h"
#include "gpio.h"
#include "interrupts.h"
#include "keyboard.h"
#include "shell.h"
#include "uart.h"

extern void console_enable_blink_cursor(void);

/* This program tests the keyboard using the console.
 * Console reads from the keyboard and displays on screen.
 * If the keyboard is not using interrupts, it drops events
 * because the screen refresh is so time consuming.
 */
#define USE_INTERRUPTS
void main(void)
{
    gpio_init();
    uart_init();
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
    console_init(30, 80, GL_SILVER, GL_INDIGO);
    shell_init(keyboard_read_next, console_printf);

    interrupts_init();
#ifdef USE_INTERRUPTS
    keyboard_use_interrupts();
    console_enable_blink_cursor();
#endif
    interrupts_global_enable();

    shell_run();
}
