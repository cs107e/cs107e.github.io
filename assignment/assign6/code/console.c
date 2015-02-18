#include "system.h"
#include "led.h"
#include "timer.h"
#include "gpio.h"
#include "fb.h"
#include "gfx.h"
#include "rpi_logo.h"

void notmain() {
  gpio_init();
  led_init();
  timer_init();
  fb_init();
  while (1) {led_toggle();}
}

