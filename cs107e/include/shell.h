#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>

/*
 * Interface to the CS107E shell. You implement the beginnings
 * of your shell in assignment 5 and complete it in assignment 7.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Last update: February 2019
 */


/*
 * This typedef gives a nickname to the type of function pointer used as the
 * the shell output function.  A formatted_fn_t function has one fixed
 * parameter (format string) and variable arguments to follow. The return
 * value is of type int.
 */
typedef int (*formatted_fn_t)(const char *format, ...) __attribute__((format(printf, 1, 2)));

/*
 * `shell_init`: Required initialization for shell
 *
 * One argument is a function pointer `print_fn`. The shell will call
 * this function whenever it wants to output text.  By supplying
 * a different function, you control how/where the output is displayed.
 *
 * Example usage:
 *   * `shell_init(printf)`
 *   * `shell_init(console_printf)`
 */
void shell_init(formatted_fn_t print_fn);

/*
 * `shell_bell`: audio/visual beep
 *
 * Sends an Ascii BEL character '\a' over the uart. This gives
 * an audio or visual beep depending on your terminal settings.
 * https://en.wikipedia.org/wiki/Bell_character
 */
void shell_bell(void);

/*
 * `shell_readline`: read function of shell read-eval-print loop
 *
 * Reads a single line of input from the keyboard.
 *
 * Reads characters typed on the keyboard and stores them into `buf`.
 * Reading stops when the user types Return ('\n') or when `buf` is
 * full (`bufsize` - 1), whichever comes first. A null-terminator is
 * written to the end of the contents in `buf`.
 * The ending newline is discarded (not written to buf).
 *
 * When the user types backspace (\b):
 *   If there are any characters currently in the buffer, deletes the last one.
 *   Otherwise, calls `shell_bell`.
 */
void shell_readline(char buf[], size_t bufsize);

/*
 * `shell_evaluate`: eval function of shell read-eval-print loop
 *
 * Parses line and execute command.
 *
 * Parsing proceeds as follows:
 *   - Divide the line into an array of tokens. A token consists
 *     of a sequence of non-space chars.  Ignore/skip all whitespace
 *     in between tokens as well as leading and trailing whitespace.
 *     Whitespace includes space, tab, and newline.
 *   - The first token is the name of the command to execute, the
 *     subsequent tokens are the arguments to the command.
 * After parsing, execute the command:
 *   - Find the function pointer associated with the command name.
 *   - If no such command is found, give error:
 *           error: no such command 'binky'.
 *   - Otherwise, execute the function, passing array of arguments.
 *
 * Returns the result of the call to the command function, or -1 if no
 * command was executed.
 */
int shell_evaluate(const char *line);

/*
 * `shell_run`: Top level shell interface.
 *
 * Main function of the shell module. Must be preceded by calls
 * to `shell_init` and `keyboard_init`.
 *
 * Enters a read-eval-print loop that repeatedly cycles between `shell_readline`
 * and `shell_evaluate`. This function never returns.
 */
void shell_run(void);


#endif
