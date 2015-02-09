#include "gpio.h"
#include "led.h"

void notmain() {
  led_init();
  gpio_init();

  gpio_set_pullup(GPIO_PIN23);
  gpio_set_output(GPIO_PIN23);

  while (1) {
    int i;
    gpio_pin_write(GPIO_PIN23, 0);
    for (i = 0; i < 100; i++) {}
    gpio_pin_write(GPIO_PIN23, 1);
    for (i = 0; i < 100; i++) {}
  } 
}
