#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "demand.h"

void FFatal(const char *file, int line, const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        fprintf(stderr, "FATAL ERROR: %s:%d: errno %d = %s\n",
                        file, line, errno, strerror(errno));
        vfprintf(stderr, fmt, args);
        va_end(args);
        exit(1);
}
