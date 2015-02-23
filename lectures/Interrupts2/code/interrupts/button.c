#include "gpio.h"
#include "led.h"
#include "gfx.h"
#include "ps2.h"

#define LINE_SIZE 59

void wait_for_press() {
  led_toggle();
  while (gpio_pin_read(GPIO_PIN23) == 0) {}
  while (gpio_pin_read(GPIO_PIN23) == 1) {}
}

void notmain() {
  led_init();
  gpio_init();
  gfx_init();

  unsigned int index = 0;
  char line[60];
  line[0] = 0;
  
  gpio_set_input(GPIO_PIN23);
  gpio_set_pullup(GPIO_PIN23);

  while (1) {
    gfx_clear();
    gfx_draw_string(0xFFFFFFFF, 20, 20, "Button pushes: ");
    gfx_draw_string(0xFFFFFFFF, 210, 20, line);
    gfx_draw();
    wait_for_press();
    line[index] = '+';
    line[++index] = 0;
  }
}
