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
 * These functions support same conversion options in format
 * string, but differ slightly in how the function is called
 * or what it does with the output string, e.g., whether it is
 * sent to the UART peripheral (printf) or written into the
 * destination buffer (snprintf, vsnprintf).
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
