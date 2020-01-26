// extra functions needed by libc

#ifndef EXTRAFUNCTIONS_LIBC
#define EXTRAFUNCTIONS_LIBC

#include<printf.h>
#include<malloc.h>
#include "sys/time.h"

time_t time(time_t *tloc);

struct tm * localtime(const time_t *clock);

// _gettimeofday will return 0,
// which will probably make file times incorrect
int _gettimeofday( struct timeval *tv, void *tzvp ) {
    printf("in _gettimeofday\n");
    return 0;
}

// _sbrk just calls malloc, which is probably not grea
// // _sbrk just calls malloc, which is probably not great...
void *_sbrk (int nbytes) {
    printf("called _sbrk...\n");
    return malloc(nbytes);
}

#endif

