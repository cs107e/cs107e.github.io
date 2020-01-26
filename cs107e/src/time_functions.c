// extra functions needed by libc

#include<printf.h>
#include<malloc.h>
#include "sys/time.h"
#include "time.h"

char *strcpy(char * dst, const char * src);

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

// _sbrk just calls malloc, which is probably not grea
// // _sbrk just calls malloc, which is probably not great...
void *_sbrk (int nbytes) {
    printf("called _sbrk...\n");
    return malloc(nbytes);
}
