#include "gpio.h"
#include "keyboard.h"
#include "uart.h"
#include "printf.h"
#include "timer.h"

#define ESC_SCANCODE 0x76


static void test_keyboard_scancodes(void) 
{
    printf("\nNow reading single scancodes. Type ESC to finish this test.\n");
    while (1) {
        unsigned char scancode = keyboard_read_scancode();
        printf("[%02x]", scancode);
        if (scancode == ESC_SCANCODE) break;
    }
    printf("\nDone with scancode test.\n");
}

static void test_keyboard_sequences(void)
{
    printf("\nNow reading scancode sequences (key actions). Type ESC to finish this test.\n");
    while (1) {
        key_action_t action = keyboard_read_sequence();
        printf("%s [%02x]\n", action.what == KEY_PRESS ? "  Press" :"Release", action.keycode);
        if (action.keycode == ESC_SCANCODE) break;
    }
    printf("Done with scancode sequences test.\n");
}

void main(void) 
{
    gpio_init();
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
    uart_init();

    test_keyboard_scancodes();
    timer_delay_ms(500);
    test_keyboard_sequences();
    printf("All done!\n");
    uart_putchar(EOT);
}

