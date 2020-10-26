#include "gpio.h"
#include "printf.h"
#include "uart.h"
#include "keyboard.h"
#include "timer.h"
#include "ps2.h"
#include "gpioextra.h"

#include <stdbool.h>
enum { START_BIT = 0, STOP_BIT = 1 };
static unsigned CLK = GPIO_PIN3;
static unsigned DATA = GPIO_PIN4;


static void wait_for_falling_clock_edge(void)
{
    while (gpio_read(CLK) == 0) {}
    while (gpio_read(CLK) == 1) {}
}

// Please don't use this in your own code: you should figure out
// how to compute/check parity using simpler logic. We'll see
// how this works when we cover Arithmetic and numeric representations
// at the end of the quarter.
bool has_odd_parity(unsigned char code) {
  int parity = 1;
  while (code) {
      parity = !parity;
      code &= code - 1;
  }
  return parity != 0;
}

static void write_bit(int nbit, unsigned char code)
{
    switch (nbit) {
        case 1:
            gpio_write(DATA, START_BIT);
            break;
        case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
            gpio_write(DATA, (code >> (nbit-2)) & 1);
            break;
        case 10:
            gpio_write(DATA, has_odd_parity(code));
            break;
        case 11:
            gpio_write(DATA, STOP_BIT);
            break;
    }
}

void ps2_write(unsigned char command)
{
    gpio_set_output(CLK);   // swap GPIO from read to write
    gpio_set_output(DATA);
    gpio_write(CLK, 0);     // bring clock line low to inhibit communication
    timer_delay_us(200);    // hold time > 100 us
    gpio_write(DATA, 0);    // initiate request-to-send, this is START bit
    gpio_write(CLK, 1);
    gpio_set_input(CLK);    // release clock line, device will now pulse clock for host to write

    for (int i = 2; i <= 11; i++) {  // START bit sent above, now send bits 2-11
        wait_for_falling_clock_edge();
        write_bit(i, command);
    }

    gpio_set_input(DATA);   // done writing, exit from request-to-send
    wait_for_falling_clock_edge();
}


