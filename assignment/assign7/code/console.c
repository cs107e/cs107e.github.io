#include "system.h"
#include "led.h"
#include "timer.h"
#include "gpio.h"
#include "fb.h"
#include "gfx.h"
#include "keyboard.h"
#include "rpi_logo.h"

/* Fill in your console here. It should be almost identical to your console
   for assignment 6, with the exception that calls to keyboard_read_char()
   do not block and should only be made if keyboard_has_char() returns 
   true. */

void notmain() {
  gpio_init();
  led_init();
  timer_init();
  fb_init();
  keyboard_init();
  while (1) {led_toggle();}
}

