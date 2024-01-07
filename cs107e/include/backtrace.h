#ifndef BACKTRACE_H
#define BACKTRACE_H

/*
 * Functions for harvesting a debugging backtrace from the call stack.
 *
 * Students implement this module in assignment 4.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdint.h>

/*
 * Type: `frame_t`
 *
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

/*
 * `backtrace`
 *
 * Gathers a backtrace for the calling program and writes the
 * call stack information into the array pointed to by `f`.
 *
 * A backtrace is the sequence of currently active function calls. Each element
 * in the array `f` is of type `frame_t` (struct described above). Each struct
 * represents a caller who has a frame on the stack.
 *
 * The `max_frames` argument specifies the maximum number of frames that can be
 * stored in the array f.  If the backtrace extends more than `max_frames`,
 * only the `max_frames` most recent calls are stored into the array.
 *
 * The return value of the function is the count of frames written to `f`.
 *
 * @param f            array in which to write stack frames
 * @param max_frames   maximum number of frames that can be stored in array
 * @return             count of frames written to array
 */
int backtrace(frame_t f[], int max_frames);

/*
 * `print_frames`
 *
 * Given an array of frames that has previously been filled in by a call to
 * backtrace(), `print_frames` prints the backtrace information, one line per
 * frame, using this format:
 *
 *     #0 0x85f8 at malloc+132
 *     #1 0x8868 at strndup+28
 *     #2 0x8784 at main+24
 *
 * @param f     array of stack frames
 * @param n     number of frames in array
 */
void print_frames(frame_t f[], int n);

/*
 * `print_backtrace`
 *
 * Convenience function that calls `backtrace` and `print_frames` to display
 * stack frames of currently executing program.
 */
void print_backtrace(void);

/*
 * `name_of`
 *
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
 *
 * @param fn_start_addr     numeric address of instruction in memory
 * @return                  pointer to string of function's name or
 *                          constant string "???" if name not available
 */
const char *name_of(uintptr_t fn_start_addr);

#endif
