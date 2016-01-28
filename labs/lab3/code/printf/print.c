/* Use the C standard headers so printf is declared. */
#include <stdio.h>
#include "uart.h"
#include "timer.h"

int main() {

  uart_init();

  /* Initial delay so if you print something after this,
     then you have time to run `screen` before it prints. */
  delay(10);

  while (1) {
    printf("hello\r\n");
    delay(1);
  }

  return 0;
}
