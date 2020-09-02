#include "uart.h"
#include "keyboard.h"
#include "printf.h"
#include "shell.h"


void main(void) 
{
    uart_init();
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
    shell_init(printf);

    shell_run();
}
