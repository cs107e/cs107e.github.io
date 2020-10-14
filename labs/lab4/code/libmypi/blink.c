#include "gpio.h"
#include "timer.h"

void main(void) {

  gpio_init();
  timer_init();

  gpio_set_output(GPIO_PIN20);
  while(1) {
      gpio_write(GPIO_PIN20, 1);
      timer_delay(1);
      gpio_write(GPIO_PIN20, 0);
      timer_delay(1);
  } 
}
