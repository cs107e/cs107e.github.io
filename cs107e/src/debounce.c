/* 
 * File: debounce.c
 * ----------------
 *
 * This version (March 2018) not fully robust, Pat has updated code.
 */

#include "gpio.h"
#include "debounce.h"

static int button[64];
static int oldbutton[64];

void debounce_init(int pin) {
    button[pin] = oldbutton[pin] = gpio_read(pin);
}

int debounce(int pin) {
    int val = gpio_read(pin);

    if (oldbutton[pin] == val) {
        button[pin] = val;
    }

    oldbutton[pin] = val;

    return button[pin];
}

