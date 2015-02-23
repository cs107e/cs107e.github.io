#include "gpio.h"
#include "led.h"
#include "gfx.h"
#include "ps2.h"

#define SIZE 60
static char string[SIZE + 1];
static unsigned len = 0;
static unsigned ignore_count = 0;
void notmain() {
  led_init();
  gpio_init();
  gfx_init();
  
  gpio_set_input(GPIO_PIN23); 
  gpio_set_pullup(GPIO_PIN23); 
  gpio_set_input(GPIO_PIN24); 
  gpio_set_pullup(GPIO_PIN24); 

  string[0] = 'Z';
  string[1] = 0;
  len = 1;
  gfx_draw_letter(0xFFFFFFFF, 200, 20, '!');
  
  while (1) {
    /* Code removed as it's part of assignment 6. */
  }

}
