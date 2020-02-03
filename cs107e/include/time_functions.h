// extra functions needed by libc

#ifndef EXTRAFUNCTIONS_LIBC
#define EXTRAFUNCTIONS_LIBC

#include <sys/time.h>
#include <time.h>

time_t time(time_t *tloc);

struct tm * localtime(const time_t *clock);

// _gettimeofday will return 0,
// which will probably make file times incorrect
int _gettimeofday( struct timeval *tv, void *tzvp );

#endif


