/* File: compare.c
 * ---------------
 * Use gpio pin as comparator.
 */

#include "gpio.h"
#include "printf.h"
#include "uart.h"

static const gpio_id_t COMPARE = GPIO_PB3;

void main(void) {
    gpio_init();
    uart_init();
    printf("Using gpio pin as comparator. Is voltage < or > 1.67V?\n");

    gpio_set_input(COMPARE);
    int last = -1;
    while (1) {
        int cur = gpio_read(COMPARE);
        if (cur != last) printf("Pin is %s\n", (cur? "HIGH" : "LOW"));
        last = cur;
    }
}
