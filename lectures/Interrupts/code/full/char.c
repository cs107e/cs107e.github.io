#include "gpio.h"
#include "led.h"
#include "gfx.h"
#include "ps2.h"

#define LINE_SIZE 59

void wait_for_clock() {
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
  gpio_set_input(GPIO_PIN24);
  gpio_set_pullup(GPIO_PIN23);
  gpio_set_pullup(GPIO_PIN24);
  //  while (1) {}
  while (1) {
    wait_for_clock();
    line[index] = '+';
    line[++index] = 0;
    gfx_clear();
    gfx_draw_string(0xFFFFFFFF, 20, 20, "Button pushes: ");
    gfx_draw_string(0xFFFFFFFF, 210, 20, line);
    gfx_draw();
  }
  
  while (1) {
    unsigned char ch = 0;
    int pinval;

    wait_for_clock();
    pinval = gpio_pin_read(GPIO_PIN24);
    if (pinval != 0) {
      //continue;
    }    

    int i;
    for (i = 0; i < 8; i++) {
      wait_for_clock();
      ch |= gpio_pin_read(GPIO_PIN24) << i;
    }

    wait_for_clock();
    wait_for_clock();

    ch = 'z'; //lookup_table[ch];
    if (index < LINE_SIZE) {
      line[index] = ch;
      line[index++] = 0;
    }
    gfx_clear();
    gfx_draw_string(0xFFFFFFFF, 20, 20, line);
    gfx_draw();
  }
 
}
