#include <stabs.h>
#include <malloc.h>

extern int __bss_start__;
extern int __bss_end__;
extern int __stab_start__;

static unsigned char *mem_ptr = (unsigned char *)&__bss_end__;

void bt() {
  // FIXME: Write some code here! And maybe elsewhere, too.
}

#ifndef REDZONE_MALLOC
void *malloc(size_t n) {
  n = roundup(n, 8);
  void *alloc = (void *)mem_ptr;
  mem_ptr += n;

  return alloc;
}
#else
void *_rzmalloc(size_t n, const char *file, int line, const char *func) {
  n = roundup(n, 8);
  void *alloc = (void *)mem_ptr;
  mem_ptr += n;

  // FIXME: Change this function to be a redzone allocator!

  return alloc;
}

int rz_check() {
  // FIXME: Check the integrity of all redzones.
  return 0;
}
#endif

void main() {
  // TODO: Run some tests here to ensure your code works.
}
