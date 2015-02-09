#include "gpio.h"
#include "led.h"
#include "gfx.h"
#include "ps2.h"

void notmain() {
  led_init();
  gpio_init();
  gfx_init();

  gfx_draw_letter(0xFFFFFFFF, 20, 20, 'Z');
}
