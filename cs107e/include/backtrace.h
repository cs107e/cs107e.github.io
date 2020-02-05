#ifndef BACKTRACE_H
#define BACKTRACE_H

#include <stdint.h>

/*
 * Functions for harvesting a debugging backtrace from the stack.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Mon Feb  5 20:02:27 PST 2018
 */

/* Type: frame_t
 * -------------
 * This struct stores the information for a function who has a frame on
 * the current stack. 
 *
 * The `name` field is the name of the function as found by `name_of`.
 *
 * The `resume_addr` field is taken from saved lr in the callee. The saved lr
 * (sometimes referred to as "return address") is the address of the
 * instruction in the caller's sequence of instructions where control will
 * resume after the callee returns.  The `uintptr_t` type is an unsigned integer
 * of the appropriate size to store an address. This type is used to hold
 * an address that you intend to treat numerically.
 *
 * The `resume_offset` is the number of bytes from the start of the function to
 * the `resume_addr`. This offset represents how far control has advanced into
 * the caller function before it invoked the callee.
 */
typedef struct {
    const char *name;
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

/* Function: name_of
 * -----------------
 * The argument to `name_of` is the numeric address in memory of the first
 * instruction of a function and `name_of` returns the name of that
 * function.
 *
 * When compiling with the `-mpoke-function-name` flag, each function's
 * name is written into the text section alongside its instructions.
 * `name_of` finds a function's name by looking in the appropriate
 * location relative to the function's start address. The `uintptr_t` type 
 * is an unsigned integer of the appropriate size to store an address. 
 * This type is used to hold an address that you intend to treat numerically.
 * 
 * If no name is available for the given address, `name_of` returns 
 * the constant string "???"
 */
const char *name_of(uintptr_t fn_start_addr);


#endif
