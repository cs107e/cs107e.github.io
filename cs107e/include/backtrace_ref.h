#ifndef BACKTRACE_REF
#define BACKTRACE_REF
#include "backtrace.h"
int ref_backtrace(frame_t f[], int max_frames);
void ref_print_frames(frame_t f[], int n);
void ref_print_backtrace(void);
const char *ref_name_of(uintptr_t fn_start_addr);
#endif
