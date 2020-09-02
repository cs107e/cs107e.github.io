#include "gpio.h"
#include "keyboard.h"
#include "console.h"
#include "shell.h"

// Main function for assignment 6 (console)
// Runs your shell from assignment 5, now in graphical form
// You should not need to modify this program

#define NUM_ROWS 20
#define NUM_COLS 40

void main(void)
{
    gpio_init();
    console_init(NUM_ROWS, NUM_COLS);
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);

    shell_init(console_printf);
    shell_run();
}
