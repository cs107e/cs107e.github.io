#ifndef PRINTF_H
#define PRINTF_H

/*
 * Functions for printing formatted strings.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Sat Jan 27 11:27:13 PST 2018
 */
#include <stdarg.h>
#include <stddef.h>

/* These three functions from the printf family are used to produce 
 * formatted output. All three functions accept format strings
 * using the same kind of conversions, but differ slightly in
 * how they are called or where they put the output, e.g.: whether
 * the output is written to the Raspberry Pi UART (printf) or stored
 * as a string in the client's buffer (snprintf, vsnprintf).
 *
 * The supported format conversions are
 *   %c    single character
 *   %s    string
 *   %d    signed decimal integer
 *   %x    unsigned hexadecimal integer
 *   %p    pointer (printed as an address)
 *   %%    used to output a single percent character
 *
 * The %d and %x formats support an optional field width.
 * 
 * Other fancy features of printf (padding with spaces,
 * left versus right justification, precision, etc.) are not supported.
 * Any format conversion other than the supported ones listed above 
 * is considered invalid. The function's behavior for an invalid 
 * format conversion is undefined.
 */

/* 
 * `buf` is the destination of the formatted string
 * `bufsize` is the size of `buf`
 * `format` is a character string which contains ordinary characters (copied 
 * to `buf`) and format conversions (cause conversion and copying of the next 
 * successive argument to `buf`).
 *
 * Returns the number of characters written if entire formatted string 
 * fits in `buf`; otherwise returns the number of characters it would 
 * have written if there were space.
 */
int vsnprintf(char *buf, size_t bufsize, const char *format, va_list args);

/* 
 * `buf` is the destination of the formatted string
 * `bufsize` is the size of `buf`
 * `format` is a character string which contains ordinary characters (copied 
 * to `buf`) and format conversions (cause conversion and copying of the next 
 * successive argument to `buf`).
 *
 * Returns the number of characters written if entire formatted string 
 * fits in `buf`; otherwise returns the number of characters it would 
 * have written if there were space.
 */
int snprintf(char *buf, size_t bufsize, const char *format, ...) __attribute__ ((format (printf, 3, 4)));

/* `format` is a character string which contains plain characters (copied 
 * to the UART) and format conversions (cause conversion and printing of 
 * the next successive argument to the UART).
 *
 * Returns the number of characters written to the output.
 */
int printf(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

#endif
