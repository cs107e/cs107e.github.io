# 1 "main.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "main.c"
# 1 "util.h" 1



int add_char_nums(char x, char y);
# 2 "main.c" 2

int main(int argc, char *argv[]) {
  if (argc < 3) {
    return -1;
  }

  return add_char_nums(argv[1][0], argv[2][0]);
}
