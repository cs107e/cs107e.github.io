#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>
#include <stddef.h>

void *sys_memset(void *s, int c, size_t n);

// asm routines
void syscall_read(int fd, char *s, int count);
void syscall_write(int fd, const char *s, int count);
void syscall_exit(int code);

unsigned long sys_instret(void);
unsigned long sys_cycles(void);

#endif
