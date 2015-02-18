#include "system.h"
#include "led.h"
#include "timer.h"
#include "gpio.h"

void notmain() {
  gpio_init();
  led_init();
  timer_init();
  
}

