#include "gpio.h"
#include "keyboard.h"
#include "uart.h"
#include "printf.h"

#define ESC_SCANCODE 0x76

void main(void) 
{
    gpio_init();
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
    uart_init();

    printf("\nNow reading single scancodes. Type ESC to finish this test.\n");
    while (1) {
        unsigned char scancode = keyboard_read_scancode();
        printf("[%02x]\n", scancode);
        if (scancode == ESC_SCANCODE) break;
    }
    printf("\nDone with scancode test.\n");
}
