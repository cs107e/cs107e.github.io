/**
 *  Uses GPIO events to detect falling edges on GPIO
 *  pin 23 (PS/2 clock line) and toggle the activity LED.
 *
 *  Visual demonstration of using GPIO event hardware support.
 *
 *  Author: Philip Levis <pal@cs.stanford.edu>
 *  Date: 4/26/16
 */

#include "gpio.h"
#include "gpioextra.h"
#include "pi.h"

const int CLK = GPIO_PIN3;

void wait_for_clock(void) {
  // wait for CLK event
  while(gpio_check_event(CLK) == 0) {}
  // must clear event
  gpio_clear_event(CLK);
}

void main(void) {
    gpio_init();

    gpio_set_input(CLK);
    gpio_set_pullup(CLK);

    // generate event on falling edge of clock
    gpio_enable_event_detection(CLK, GPIO_DETECT_FALLING_EDGE);

    while (1) {
        wait_for_clock();
        pi_led_toggle(PI_ACT_LED);
    }
}
