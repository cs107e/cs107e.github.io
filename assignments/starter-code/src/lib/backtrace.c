#include "backtrace.h"
#include "printf.h"

const char *name_of(uintptr_t fn_start_addr)
{
    // TODO: Fill in your code here.
    return "???";
}

int backtrace (frame_t f[], int max_frames)
{
    // TODO: Fill in your code here.
    return 0;
}

void print_frames (frame_t f[], int n)
{
    for (int i = 0; i < n; i++)
        printf("#%d 0x%x at %s+%d\n", i, f[i].resume_addr, f[i].name, f[i].resume_offset);
}

void print_backtrace (void)
{
    int max = 50;
    frame_t arr[max];

    int n = backtrace(arr, max);
    print_frames(arr+1, n-1);   // print frames starting at this function's caller
}
