/* File: compare.c
 * ---------------
 * Use gpio pin as comparator.
 */

#include "gpio.h"
#include "gpio_extra.h"
#include "printf.h"
#include "uart.h"

void main(void) {
    gpio_id_t pin = GPIO_PB3;

    gpio_init();
    uart_init();
    printf("Using gpio pin %s as comparator. Is voltage < or > 1.67V?\n", gpio_get_name_for_id(pin));

    gpio_set_input(pin);
    int last = -1;
    while (1) {
        int cur = gpio_read(pin);
        if (cur != last) printf("Pin is %s\n", (cur? "     HIGH" : "LOW"));
        last = cur;
    }
}
