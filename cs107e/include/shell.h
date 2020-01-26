#ifndef SHELL_H
#define SHELL_H

/*
 * Interface to the CS107E shell. You implement the beginnings
 * of your shell in assignment 5 and complete it in assignment 7.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Last update: February 2019
 */

typedef int (*formatted_fn_t)(const char *format, ...);

/*
 * `shell_init`: Required initialization for shell
 *
 * Takes a *function pointer* `printf_fn` that is used
 * to configure where the shell directs its output.
 *
 * Arguments:
 *   * `printf_fn` - used for the formatted output
 *
 * Example usage:
 *   * `shell_init(printf)`
 *   * `shell_init(console_printf)`
 */
void shell_init(formatted_fn_t print_fn);

/*
 * `shell_bell`: audio/visual beep
 *
 * Send Ascii BEL character '\a' over the uart. 
 * https://en.wikipedia.org/wiki/Bell_character
 */
void shell_bell(void);

/*
 * `shell_readline`: read function of shell read-eval-print loop
 *
 * Reads a single line of input from the user. Reads the characters typed on
 * the keyboard and stores them into `buf`. Reading stops when the user
 * types Return ('\n') or when `buf` is full (`bufsize` - 1), whichever
 * comes first. The contents written to `buf` are null-terminated. 
 * The ending newline is discarded (not written to buf).
 *
 * When the user types backspace (\b):
 *   If there are any characters currently in the buffer, deletes the last one.
 *   Otherwise, calls `shell_bell`.
 */
void shell_readline(char buf[], int bufsize);

/*
 * `shell_evaluate`: eval function of shell read-eval-print loop
 *
 * Parses a line as follows:
 *   - If the line is empty/all space, do nothing
 *   - Otherwise, tokenize the line. A token is a sequence of non-space
 *     characters. There may be arbitrary whitespace in between tokens, 
 *.    as well as leading and trailing whitespace. Whitespace includes
 *     space, tab, and newline.
 *   - The first token is the command name.
 *   - Looks up the command function associated with the command name.
 *   - If no such command is found, prints an error:
 *           error: no such command 'binky'.
 *   - Otherwise, executes the command function, passing the rest of 
 *     the tokens as arguments.
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
 * Enters a read-eval-print loop built of out of `shell_readline`, 
 * `shell_evaluate`, and the `print_fn` argument to `shell_init`.
 * This function never returns.
 */
void shell_run(void);


#endif
