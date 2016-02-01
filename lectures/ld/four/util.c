#include "util.h"

/* static int __attribute__ ((noinline)) add_char_nums(char x, char y) { */
int add_char_nums(char x, char y) {
  // ASCII '0' is 48
  int a = (int)(x) - 48;
  int b = (int)(y) - 48;
  return a + b;
}

int add_to_one(char x) {
  return add_char_nums(x, '1');
}
