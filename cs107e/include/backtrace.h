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
 * This struct stores the information for a function that has a frame on the
 * call stack. The `resume_addr` is taken from saved ra in the callee frame.
 * The return address in the caller's sequence of instructions where control
 * will resume after the callee returns. The type `uintptr_t` is an
 * unsigned int/long of the appropriate bitwidth to store an address. The
 * `uintptr_t` type is used for an address that you intend to treat numerically.
 */
typedef struct {
    uintptr_t resume_addr;
} frame_t;

/*
 * `backtrace_gather_frames`
 *
 * Gathers a backtrace by harvesting frames from the current call stack and
 * storing into the frame array `f`.
 *
 * A backtrace is the sequence of currently active function calls. Each element
 * in the array `f` is of type `frame_t` (struct described above). Each struct
 * represents a caller who has a frame on the stack.
 *
 * The `max_frames` argument specifies the maximum number of frames to harvest.
 * If the current call stack contains more than `max_frames`, only the
 * `max_frames` topmost calls are stored into the array.
 *
 * The function returns the count of frames written to `f`.
 *
 * @param f            array in which to write stack frames
 * @param max_frames   maximum number of frames that can be stored in array f
 * @return             count of frames written to array f
 */
int backtrace_gather_frames(frame_t f[], int max_frames);

/*
 * `backtrace_print_frames`
 *
 * Given an array of frames as filled in by a call to backtrace_gather_frames(),
 * this function prints one line per frame zero-indexed using format:
 *
 *      #0  0x40000124 at <label+offset>
 *      #1  0x4000004c at <label+offset>
 *      #2  0x400001d8 at <label+offset>
 *
 * If symbols are available, the label is the name of the function and offset.
 * The offset is the number of bytes from function start to resume_addr.
 * If symbols are not available,  the label is the offset of the address
 * within the .text section. See `symtab_label_for_addr` in symtab.h
 *
 *     #2 0x400001d8 at <.text+2816>
 *
 * @param f     array of stack frames
 * @param n     number of frames in array f
 */
void backtrace_print_frames(frame_t f[], int n);

/*
 * `backtrace_print`
 *
 * Convenience function that calls `backtrace_gather_frames` and
 * `backtrace_print_frames` to display current call stack.
 */
void backtrace_print(void);

#endif
