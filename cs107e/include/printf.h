#ifndef PRINTF_H
#define PRINTF_H

/*
 * Functions for printing formatted strings.
 *
 * Students implement this module in assignment 3
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdarg.h>
#include <stddef.h>

/*
 * The functions vnsprintf, snprintf, and printf construct a
 * formatted output from an input format string and arguments.
 * All three functions support same conversion options in format
 * string, but differ slightly in how each function is called
 * or what it does with the output string, e.g., whether it is
 * sent to the UART peripheral (printf) or written into the
 * destination buffer (snprintf, vsnprintf).
 *
 * The supported format conversion codes are
 *   %c    single character
 *   %s    string
 *   %d    signed decimal integer (%ld long decimal)
 *   %x    unsigned hexadecimal integer (%lx long hex)
 *   %p    pointer
 *   %%    used to output a single percent character
 *
 * Each formatting code also allows an optional field width, such
 * as %25s or %8lx. The field width enforces a minimum number of
 * characters for this conversion. The output will be left-padded
 * with spaces up to the field width. If the output of the conversion
 * is hexadecimal, character '0' is used as pad instead of space char.
 *
 * This version of printf does not support the many fancy features of
 * the standard library printf (no justification, no precision, no octal,
 * no floating point, etc.). Format conversions other than the supported
 * list are considered invalid. The behavior of printf for an invalid
 * format is undefined.
 */

/*
 * `vsnprintf`
 *
 * Constructs a formatted output string from an input string and a va_list
 * of arguments. Writes output string to the destination buffer.
 *
 * @param buf       destination buffer where to write output string
 * @param bufsize   size of destination buffer (output truncated to fit if needed)
 * @param format    format for output string. May contain ordinary characters
 *                  (copied to destination as-is) and format conversions (next
 *                  argument converted and copied to destination)
 * @param args      list of arguments to be converted
 * @return          count of characters written if entire formatted string
 *                  fits in destination; otherwise count of characters it would
 *                  have written if there were space.
 */
int vsnprintf(char *buf, size_t bufsize, const char *format, va_list args);

/*
 * `snprintf`
 *
 * Constructs a formatted output string from an input string and variable
 * arguments. Writes output string to the destination buffer.
 *
 * @param buf       destination buffer where to write output string
 * @param bufsize   size of destination buffer (output truncated to fit if needed)
 * @param format    format for output string. May contain ordinary characters
 *                  and format conversions
 * @param ...       variable arguments to be converted
 * @return          count of characters written if entire formatted string
 *                  fits in destination; otherwise count of characters it would
 *                  have written if there were space.
 */
int snprintf(char *buf, size_t bufsize, const char *format, ...) __attribute__((format(printf, 3, 4)));

/*
 * `printf`
 *
 * Constructs a formatted output string from an input string and arguments.
 * Writes output string to UART.
 *
 * @param format    format for output string. May contain ordinary characters
 *                  and format conversions
 * @param ...       variable arguments to be converted
 * @return          count of characters written to UART
 */
int printf(const char *format, ...) __attribute__((format(printf, 1, 2)));

#endif
