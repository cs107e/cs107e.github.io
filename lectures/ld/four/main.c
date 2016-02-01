#include "util.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    return -1;
  }

  return add_char_nums(argv[1][0], argv[2][0]);
}
