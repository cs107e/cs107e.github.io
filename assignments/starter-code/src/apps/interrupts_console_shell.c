#include "console.h"
#include "interrupts.h"
#include "keyboard.h"
#include "shell.h"
#include "timer.h"
#include "uart.h"

#define NROWS 20
#define NCOLS 40


void main(void) 
{
    interrupts_init();
    gpio_init();
    timer_init();
    uart_init();
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
    console_init(NROWS, NCOLS);
    shell_init(console_printf);
    interrupts_global_enable(); // everything fully initialized, now turn on interrupts

    shell_run();
}
