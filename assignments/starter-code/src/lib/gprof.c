#include "armtimer.h"
#include "malloc.h"
#include "console.h"
#include "gprof.h"

/* Interval for profiler timer interrupts */
#define GPROF_TIMER_INTERVAL 0x10

void gprof_init(void) 
{ 
    // FIXME: Set up timer interrupts, attach handler, initialize counts
}

void gprof_on(void) 
{
    // FIXME: Turn on profiling and reset counts
}

void gprof_off(void) 
{
    // FIXME: Turn off profiling
}

bool gprof_is_active(void) 
{
    // FIXME: Report whether profiling is on
    return false;
}

void gprof_dump(void) 
{
    console_printf("     PC     |  COUNT    \n");
    console_printf("************************\n");
    /* FIXME: print out the instructions with 10 highest counts using:
       console_printf("%p %s+%d: %d\n", address, fn_name, offset, count); 
    */
}

bool gprof_handler(unsigned int pc) 
{
    // FIXME: Increment the count for the given address
    return false;
}
