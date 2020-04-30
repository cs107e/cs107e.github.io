#include "gpio.h"
#include "timer.h"

void main() {

  gpio_init();
  timer_init();

  gpio_set_function(GPIO_PIN20, GPIO_FUNC_OUTPUT);
  while(1) {
      gpio_write(GPIO_PIN20, 1);
      timer_delay(1);
      gpio_write(GPIO_PIN20, 0);
      timer_delay(1);
  } 
}
