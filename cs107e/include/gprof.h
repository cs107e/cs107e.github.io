#ifndef GPROF_H
#define GPROF_H

#include <stdbool.h>


/* Initialize profiler */
void gprof_init(void);

/* Turn on profiling */
void gprof_on(void);

/* Turn off profiling */
void gprof_off(void);

/* Report whether profiling on */
bool gprof_is_active(void);

/* Print the profiler results */
void gprof_dump(void);

/* This will be implemented in shell */
int cmd_profile(int argc, const char *argv[]);


#endif
