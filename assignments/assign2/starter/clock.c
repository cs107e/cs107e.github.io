#include "timer.h"
#include "gpio.h"

/*
 * Returns the current system time in us.
 *
 * @return system time in microseconds
 */
unsigned int timer_get_time(void) {
  // TODO: Your code goes here.
}

/*
 * A simple busy loop that delays the program for `n` microseconds.
 *
 * @param n the number of microseconds to busy loop for
 */
void delay_us(unsigned int n) {
    unsigned int start = timer_get_time();
    while (timer_get_time() - start < n) { /* spin */ }
}

/*
 * Set GPIO pin number `pin` to the GPIO function `function`. Does not affect
 * other pins.
 *
 * @param pin the pin number to initialize
 * @param function the GPIO function to set for the pin
 */
void gpio_set_function(unsigned int pin, unsigned int function) {
  // TODO: Your code goes here.
}

/*
 * Sets GPIO pin number `pin` to the value `value`. The GPIO pin should be set
 * the output mode for this call to function correctly.
 *
 * @param pin the pin number to set or clear
 * @param value 1 if the pin should be set, 0 otherwise
 */
void gpio_write(unsigned int pin, unsigned int value) {
  // TODO: Your code goes here.
}

void main(void) {
  // TODO: Your code goes here.
}
