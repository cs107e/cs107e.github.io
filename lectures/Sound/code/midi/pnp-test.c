#include "gpio.h"
#include "timer.h"

void notmain() {
  gpio_init();
  timer_init();

  gpio_set_output(GPIO_PIN23);
  while (1) {
    gpio_pin_write(GPIO_PIN23, 0);
    timer_wait_for(1000);
    gpio_pin_write(GPIO_PIN23, 1);
    timer_wait_for(4000);
  }
}
