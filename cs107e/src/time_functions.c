// extra functions needed by libc

#include "printf.h"
#include "time_functions.h"

time_t time(time_t *tloc) {
    return 0;
}

struct tm * localtime(const time_t *clock) {
    static struct tm _tm;
    return &_tm;
}

// _gettimeofday will return 0,
// which will probably make file times incorrect
int _gettimeofday( struct timeval *tv, void *tzvp ) {
    printf("in _gettimeofday\n");
    return 0;
}

