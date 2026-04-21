/* Here is a comment */

#include "gpio.h"
#include "timer.h"

#define DELAY (1000/3)

static void blink(gpio_id_t pin) {
    gpio_write(pin, 1); // here is another comment
    timer_delay_ms(DELAY);
    gpio_write(pin, 0);
    timer_delay_ms(DELAY);
}

void main(void) {
    gpio_set_output(GPIO_PB6);
    blink(GPIO_PB6);
}
