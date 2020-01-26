#ifndef BACKTRACE_H
#define BACKTRACE_H

#include <stdint.h>

/*
 * Functions for debugging backtraces.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Mon Feb  5 20:02:27 PST 2018
 */


/* Type: frame_t
 * -------------
 * This struct stores the information for a caller function who has a
 * frame on the stack. 
 *
 * The `name` fields points to the string name of the function. That name is in
 * the text section with the instructions.  If the function name is not
 * available, the string will be "???".
 *
 * The `resume_addr` field is taken from saved lr in the callee. The saved lr
 * (sometimes referred to as "return address") is the address of the
 * instruction in the caller's sequence of instructions where control will
 * resume after the callee returns. 
 *
 * The `resume_offset` is the number of bytes from the start of the function to
 * the `resume_addr`. This offset represents how far control has advanced into
 * the caller function before it invoked the callee.
 */
typedef struct {
    char *name;
    uintptr_t resume_addr;
    int resume_offset;
} frame_t;

/* Function: backtrace
 * -------------------
 * backtrace() gathers a backtrace for the calling program and writes the
 * call stack information into the array pointed to by `f`. 
 *
 * A backtrace is the sequence of currently active function calls. Each element
 * in the array `f` is of type `frame_t` (struct described above). Each struct 
 * represents a caller who has a frame on the stack. 
 *
 * The `max_frames` argument specifies the maximum number of frames that can be
 * stored in the array f.  If the backtrace contains more than `max_frames`,
 * then the information for only the `max_frames` most recent function calls
 * are stored in the array `f`. 
 *
 * The return value of the function is the number of frames written to `f`.
 */
int backtrace(frame_t f[], int max_frames);

/* Function: print_frames
 * ----------------------
 * Given an array of frames that has previously been filled in by a call to
 * backtrace(), `print_frames` prints the backtrace information, one line per
 * frame, using this format:
 *
 *     #0 0x85f8 at malloc+132
 *     #1 0x8868 at strndup+28
 *     #2 0x8784 at main+24
 */
void print_frames(frame_t f[], int n);

/* Function: print_backtrace
 * -------------------------
 * Convenience function that calls `backtrace` and `print_frames` to display
 * stack frames of currently executing program.
 */
void print_backtrace(void);

#endif
