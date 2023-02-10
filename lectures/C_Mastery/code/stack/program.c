#include "gpio.h"
#include "timer.h"
#include "printf.h"
#include "uart.h"
#include "strings.h"

void blink_act_led(void) {
    unsigned int pin = GPIO_PIN47;
    uart_putstring("Blink the ACT led on the Pi\n");
    gpio_set_output(pin);
    for (int i = 0; i < 2; i++) {
        uart_putstring("on...");
        gpio_write(pin, 1);
        timer_delay(1);
        uart_putstring("off...");
        gpio_write(pin, 0);
        timer_delay(1);
    }
    uart_putstring("Done\n");
}

bool make_trouble(int how_much) {
    char buf[5] = "abcd";
    printf("How much trouble? Writing %d bytes beyond... ", how_much);
    memset(&buf[4], 0, how_much);
    return true;
}

void main(void) {
    static int count;

    gpio_init();
    uart_init();

    printf("\n\n\n++++++++++ STARTING %s ++++++++++\n\n", __FILE__);

    blink_act_led();
    for (count = 0; count < 100; count += 4) {
        if (make_trouble(count))
            printf("success!\n");
    }
    printf("\n\n++++++++++ EXITING %s ++++++++++\n", __FILE__);
}
