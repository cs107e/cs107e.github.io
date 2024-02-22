#ifndef CONSOLE_H
#define CONSOLE_H

/*
 * Interface to a text console displayed on the screen.
 *
 * Students implement this module in assignment 6.
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Author: Philip Levis <pal@cs.stanford.edu>
 */

#include "gl.h"

/*
 * `console_init`: required initialization for console
 *
 * Initialize the console. The console text begins empty and
 * the cursor is in the home position (upper left corner).
 *
 * @param nrows       the requested number of rows in characters
 * @param ncols       the requested number of columns in characters
 * @param foreground  foreground color used for text
 * @param background  background color
 *
 * A subsequent call to console_init after the first does a reinitialization.
 */
void console_init(int nrows, int ncols, color_t foreground, color_t background);

/*
 * `console_clear`
 *
 * Clear all console text and move the cursor to home position.
 */
void console_clear(void);

/*
 * `console_printf`
 *
 * Print the formatted string to the console starting at current cursor
 * position. The arguments to this function are the same as `printf`.
 * When processing characters, interpret the following special characters:
 *
 * '\n' :  newline (move cursor down to the beginning of next line)
 * '\b' :  backspace (move cursor backwards one position)
 * '\f' :  form feed (clear all contents and move cursor to home position)
 *
 * @param format    format for output string. May contain ordinary characters
 *                  and format conversions
 * @param ...       variable arguments to be converted
 * @return          count of characters written to the console
 */
int console_printf(const char *format, ...) __attribute__((format(printf, 1, 2)));

#endif
