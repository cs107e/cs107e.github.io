/* Not used until you call printf and gpio_set_function yourself. */
#include <stdio.h>
#include "gpio.h"

#include "uart.h"
#define PUTCHAR(C) uart_putc(C)
#include "timer.h"
#define DELAY(S) delay(S)

void my_puts(char *s)
{
  while(*s)
    PUTCHAR(*s++);

  PUTCHAR('\n');
}

int main() {

  uart_init();

  DELAY(10);

  while (1) {
    my_puts("hello");
    DELAY(1);
  }

  return 0;
}
