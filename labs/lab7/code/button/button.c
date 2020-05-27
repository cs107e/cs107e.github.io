#include "gl.h"
#include "gpio.h"
#include "printf.h"
#include "uart.h"

static const int BUTTON = GPIO_PIN20;

void configure_button(void)
{

}


void wait_for_click(void)
{

}

void redraw(void)
{
  static int nrefresh = 0;
  color_t cycle[3] = {GL_RED, GL_WHITE, GL_BLUE};
  color_t c = cycle[nrefresh % 3];

  printf("Redraw iteration #%d\n", nrefresh++);

  for (int y = 0; y < gl_get_height(); y++) {
    for (int x = 0; x < gl_get_width(); x++) {
      gl_draw_pixel(x, y, c);
      
    }
  }
}



void main(void) 
{
  gpio_init();
  uart_init();
  
  gl_init(640, 480, GL_SINGLEBUFFER);
  configure_button();
  
  while (1) {
    redraw();
    wait_for_click();
  }
}
