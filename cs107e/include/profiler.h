#ifndef PROFILE_H
#define PROFILE_H

/*
 * Interface to a simple sampling profile.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    unsigned long start_ticks;
    int nsamples;
    bool active;
} profiler_status_t;

typedef struct {
    char label[64];
    uintptr_t pc;
    int count;
} profiler_entry_t;

typedef enum { GROUP_BY_FUNCTION, GROUP_BY_INSTRUCTION } grouping_t;


/*
 * `profiler_init`
 *
 * Initialize the profiler. Resets all counts to zero.
 */
profiler_status_t *profiler_init(void);

/*
 * `profiler_start`
 *
 * Start collecting samples. While active, the profiler is using
 * HSTIMER1 for timer events.
 */
void profiler_start(void);

/*
 * `profiler_stop`
 *
 * Stop collecting samples. The profiler will cease using HSTIMER1
 */
void profiler_stop(void);

/*
 * `profiler_result`
 *
 * Fills the array `entries` with the top N higest sample counts. Counts
 * are either per-instruction or sum over all instructions in a function
 * depending on how_grouped.
 */
void profiler_result(profiler_entry_t *entries, int n_entries, grouping_t how_grouped);

#endif
