#ifndef SIMPIO_H
#define SIMPIO_H

/*
 * This module defines simple functions for interacting with user
 * over uart.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>
#include <stddef.h>

/*
 * `gets_uart`
 *
 * Read characters from uart into buf, stopping at newline or bufsize,
 * whichever comes first.  Returns true if buf contains at least one character,
 * false otherwise.
 */
bool gets_uart(char buf[], size_t bufsize);

/*
 * `get_line`
 *
 * Output prompt using printf, read characters from uart, and return
 * pointer to a static buffer containing characters read or NULL if no
 * characters were input. The buffer contents will be overwritten on
 * subsequent call to get_line, client should make their own copy if
 * need to retain contents.
 */
const char *get_line(const char *prompt);

/*
 * `get_number`
 *
 * Output prompt using printf, read characters from uart, call
 * `strtonum` to convert to number. If invalid conversion, re-prompt and
 * try again. Returns numeric value of first valid input.
 */
unsigned long get_number(const char *prompt);

/*
 * `get_yes_or_no`
 *
 * Output prompt using printf, read characters from uart, convert
 * y/Y to true,  n/N. to false. If invalid conversion, re-prompt and
 * try again. Returns boolean value of first valid input.
 */
bool get_yes_or_no(const char *prompt);

#endif
