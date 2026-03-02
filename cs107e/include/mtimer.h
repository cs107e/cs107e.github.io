#ifndef MTIMER_H
#define MTIMER_H

/*
 * Functions for Mango Pi mtimer.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdint.h>

// -1 to disable
void mtimer_set_mtimecmp(uint64_t when);

void mtimer_advance_mtimecmp(uint64_t delta);

#endif
