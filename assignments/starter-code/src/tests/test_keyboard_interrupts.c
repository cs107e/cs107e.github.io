#include "interrupts.h"
#include "keyboard.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"


void test_clock_events(void)
{
    printf("Type on your PS/2 keyboard to generate clock events. You've got 10 seconds, go!\n");
    timer_delay(10);
    printf("Time's up!\n");
}


/*
 * This function tests the behavior of the assign5 keyboard
 * implementation versus the new-improved assign7 version. If using the
 * assign5 implementation, a key press that arrives while the main program
 * is waiting in delay is simply dropped. Once you upgrade your
 * keyboard implementation to be interrupt-driven, those keys should
 * be queued up and can be read after delay finishes.
 */
void test_read_delay(void)
{
    while (1) {
        printf("Test program waiting for you to press a key (q to quit): ");
        uart_flush();
        char ch = keyboard_read_next();
        printf("\nRead: %c\n", ch);
        if (ch == 'q') break;
        printf("Test program will now pause for 1 second... ");
        uart_flush();
        timer_delay(1);
        printf("done.\n");
    }
    printf("\nGoodbye!\n");
}


void main(void)
{
    gpio_init();
    timer_init();
    uart_init();
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);


    test_clock_events();  // wait 10 seconds for clock_edge handler to report clock edges
    //test_read_delay();  // what happens to keys typed while main program blocked in delay?
    uart_putchar(EOT);
}
