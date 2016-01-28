/* Use the C standard headers so printf is defined. */
#include <stdio.h>
#include "uart.h"
#include "timer.h"

int main() {

  uart_init();

  delay(10);

  while (1) {
    printf("hello\r\n");
    delay(1);
  }

  return 0;
}
