# 1 "util.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "util.c"
# 1 "util.h" 1



int add_char_nums(char x, char y);
# 2 "util.c" 2

int add_char_nums(char x, char y) {

  int a = (int)(x) - 48;
  int b = (int)(y) - 48;
  return a + b;
}
