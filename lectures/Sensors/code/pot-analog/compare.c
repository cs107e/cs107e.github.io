/* File: compare.c
 * ---------------
 * Use gpio pin as comparator.
 */

#include "gpio.h"
#include "printf.h"
#include "uart.h"

void main(void) {
    gpio_init();
    uart_init();
    printf("Using gpio pin as comparator. Is voltage < or > 3.3/2?\n");

    const gpio_id_t COMPARE = GPIO_PB3;
    gpio_set_input(COMPARE);
    int last = -1;
    while (1) {
        int cur = gpio_read(COMPARE);
        if (cur != last) printf("Pin is %s\n", (cur? "HIGH" : "LOW"));
        last = cur;
    }
}
