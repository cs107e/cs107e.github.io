#ifndef SHELL_H
#define SHELL_H

/*
 * Interface to a simple shell module.
 *
 * Students implement this module in assignment 5.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

 #include <stddef.h>

/*
 * Type: `input_fn_t`
 *
 * This typedef gives a nickname to the type of function pointer used as the
 * the shell input function.  A input_fn_t function takes no arguments and
 * returns a value of type unsigned char. `keyboard_read_next` is an example
 * of a possible shell input function.
 */
typedef unsigned char (*input_fn_t)(void);

/*
 * Type: `formatted_fn_t`
 *
 * This typedef gives a nickname to the type of function pointer used as the
 * the shell output function.  A formatted_fn_t function has one fixed
 * parameter (format string) and variable arguments to follow. The return
 * value is of type int. `printf` is an example of a possible shell
 *  output function.
 */
typedef int (*formatted_fn_t)(const char *format, ...) __attribute__((format(printf, 1, 2)));

/*
 * `shell_init`: Required initialization for shell
 *
 * The two arguments are function pointers. The `read_fn` is a function
 * to read input. The shell calls this function to get the next
 * input character. The `print_fn` is a function to print output.
 * The shell calls this function to output formatted text.
 * The client's choice of read function controls where shell reads
 * input and cclient's hoice of print function controls where shell
 * output is displayed.
 *
 * Example usage:
 *   `shell_init(keyboard_read_next, printf)`
 *   `shell_init(keyboard_read_next, console_printf)`
 *
 * @param read_fn    function to read input char
 * @param print_fn   function to output formatted text
 */
void shell_init(input_fn_t read_fn, formatted_fn_t print_fn);

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
 * Reads a single line of input from the user.
 *
 * Reads characters entered by user and stores them into `buf`.
 * Updates display to show current line as user edits.
 * Stops reading when user enters Return ('\n'). The ending newline
 * is discarded (not written to `buf`).  If user enters more
 * characters than fit in `buf` (`bufsize` - 1), those excess characters
 * are rejected (call `shell_bell`) and not written to `buf`.  A
 * null-terminator is written to the end of the contents in `buf`.
 *
 * When the user types backspace (\b):
 *   If there are any characters currently in the buffer, deletes the last one.
 *   Otherwise, calls `shell_bell`.
 *   Backspace through a tab character will not be tested (printing a tab
 *   outputs a variable number of spaces, handling of backspace does does
 *   not have to account for this)
 *
 * @param buf       destination buffer to store characters
 * @param bufsize   size of the buffer
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
 *   - If line contains no tokens (string is empty or all whitespace),
 *     no command is executed and -1 is returned.
 * After parsing, execute the command:
 *   - Find the function pointer associated with the command name.
 *   - If no such command is found, give error:
 *           error: no such command 'binky'.
 *   - Otherwise, execute the function, passing array of arguments.
 *
 * Returns the result of the call to the command function, or -1 if no
 * command was executed.
 *
 * @param line       command line to parse and execute
 * @return           result returned by command function or -1 if none
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
