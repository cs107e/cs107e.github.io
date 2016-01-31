#include "util.h"

int add_char_nums(char x, char y) {
  // ASCII '0' is 48
  int a = (int)(x) - 48;
  int b = (int)(y) - 48;
  return a + b;
}
