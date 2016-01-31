#include <stdio.h>

#include "main.h"

#define WHOAMI "Sergio"
#define UNUSED(x) (void)(x)

int main(int argc, char *argv[]) {
  UNUSED(argc);
  UNUSED(argv);

  printf("Hi! I'm " WHOAMI "! Nice to meet you.\n");
  printf("My constant is: %d\n", MY_CONSTANT);
  return 0;
}
