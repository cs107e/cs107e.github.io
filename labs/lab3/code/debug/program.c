#ifdef LOCAL_TEST

#include <stdio.h>
#define PUTCHAR(C) putchar(C)
#include <unistd.h>
#define DELAY(S) sleep(S)

#else

/* Not used until you call printf and gpio_set_function yourself. */
#include <stdio.h>
#include "gpio.h"

#include "uart.h"
#define PUTCHAR(C) uart_putc(C)
#include "timer.h"
#define DELAY(S) delay(S)

#endif

void my_puts(char *s)
{
  while(*s)
    PUTCHAR(*s++);

  PUTCHAR('\n');
}

int main() {

#ifndef LOCAL_TEST
  uart_init();
#endif

#ifdef LOCAL_TEST
  puts("hello");
#endif

  while (1) {
    my_puts("hello");
    DELAY(1);
  }

  return 0;
}
