#include "printf.h"
#include "strings.h"
#include "uart.h"

#include <stdint.h>

void main(void) {
  uart_init();
  /*
    double a = 0.1;
    double b = 0.2;
    double c = 0.3;
    double d = a + b;
    printf("0.1 + 0.2 == 0.3 ? %s\n", a + b == c ? "true" : "false");
    printf("%g\n", d);

    printf("16777224.0f == 16777225.0f ? %s\n",
            16777224.0f == 16777225.0f ? "true" : "false");
  */
  float x = 3e20;
  float pi = 3.14;

  float result = x + pi;
  // expect 300000000000000000003.14
  printf("result: %g %g %g\n", result, x, pi);
}
