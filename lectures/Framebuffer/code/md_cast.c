#include <stdio.h>

enum {
  FRAMEBUFFER_SIZE = 16
};

unsigned int framebuffer[FRAMEBUFFER_SIZE];

void main() { 
  // Cast array of 16 int into pointer to arrays of 4 ints
  unsigned int(*pixels)[4] = (unsigned int(*)[4])framebuffer; 
  pixels[1][1] = 0xff0625c8;
  pixels[2][2] = 0xffa9aaa6;
  printf("%x %x\n", 0xff0625c8, 0xffa9aaa6);
  printf("%x %x\n", framebuffer[5], framebuffer[10]);
}
