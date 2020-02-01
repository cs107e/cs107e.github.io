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

/* These three functions from the printf family construct a 
 * formatted output from an input format string and arguments.
 * All three functions accept format strings specified using the same 
 * kind of conversions, but differ slightly in how the function is
 * called or what it does with the output string, e.g., whether it is
 * sent to the Raspberry Pi UART (printf) or written into the 
 * client's buffer (snprintf, vsnprintf).
 *
 * The supported format conversions are
 *   %c    single character
 *   %s    string
 *   %d    signed decimal integer
 *   %x    unsigned hexadecimal integer (hex letters in lowercase)
 *   %p    pointer (printed as a hex address)
 *   %%    used to output a single percent character
 *
 * The %d and %x formats support an optional field width.
 * 
 * The fancier printf features (padding with spaces, justification
 * left/right, precision, etc.) are not supported.
 * All format conversions other than the supported ones listed above 
 * are considered invalid. The function's behavior for an invalid 
 * format conversion is undefined.
 */

/* 
 * Constructs a formatted output string from an input string and arguments.
 *
 * `buf` is the destination where output string is to be written
 * `bufsize` is the size of `buf`, output string will be truncated
 * if necessary to fit
 * `format` string may contain ordinary characters (copied to `buf` as-is)
 * and/or format conversions (written to `buf` after converting next 
 * argument to string).
 *
 * Returns total number of characters written if entire formatted string 
 * fits in `buf`; otherwise returns the number of characters it would 
 * have written if there were space.
 */
int vsnprintf(char *buf, size_t bufsize, const char *format, va_list args);

/* 
 * Constructs a formatted output string from an input string and arguments.
 *
 * `buf` is the destination where output string is to be written
 * `bufsize` is the size of `buf`, output string will be truncated
 * if necessary to fit
 * `format` string may contain ordinary characters (copied to `buf` as-is)
 * and/or format conversions (written to `buf` after converting next 
 * argument to string).
 *
 * Returns total number of characters written if entire formatted string 
 * fits in `buf`; otherwise returns the number of characters it would 
 * have written if there were space.
 */
int snprintf(char *buf, size_t bufsize, const char *format, ...) __attribute__ ((format (printf, 3, 4)));

/*
 * Constructs a formatted output string from an input string and arguments,
 * and outputs final string to UART.
 *
 * `format` string may contain ordinary characters (output as-is)
 * and/or format conversions (output after converting next argument to string).
 *
 * Returns the number of characters written to UART.
 */
int printf(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

#endif
