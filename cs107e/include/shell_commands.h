#ifndef COMMANDS_H
#define COMMANDS_H

typedef struct _command_struct {
    char *name;
    char *description;
    int (*fn)(int argc, const char *argv[]);
} command_t;

/*
 * cmd_echo
 * ========
 *
 * Prints its arguments and returns 0.
*/
int cmd_echo(int argc, const char *argv[]);

/*
 * cmd_help
 * ========
 *
 * When called without arguments: 
 *   Prints a list of all available commands and their descriptions.
 *
 * Example:
 *   Pi> help
 *   echo:   echo user input to the screen
 *   reboot: reboot the Raspberry Pi back to the bootloader
 *   ...
 *
 * When called with one argument
 *   Prints the description for that command, or an error message if that
 * command does exist:
 *
 *   Pi> help reboot
 *   reboot:  reboot the Raspberry Pi back to the bootloader
 *   Pi> help please
 *   error: no such command `please`.
 *
 * Ignores any arguments after the first. Returns 0 on success,
 * 1 on error (command not found).
 */
int cmd_help(int argc, const char *argv[]);

/*
 * cmd_reboot
 * ==========
 *
 * reboot the Raspberry Pi back to the bootloader using `pi_reboot`
 *
 * Ignores all arguments. This function does not return; the program
 * exits and Pi is restarted.
 */
int cmd_reboot(int argc, const char* argv[]);

/*
 * cmd_peek
 * ========
 *
 * Usage: peek [addresss]
 *
 * Prints the contents (4 bytes) of memory at address.
 * If address argument is prefixed with 0x, it is interpreted as hex, 
 * otherwise interpreted as decimal. Address must be 4-byte aligned.
 *
 * Example:
 *
 *   Pi> peek 0
 *   0x00000000:  e59ff030
 *
 * If the address argument is missing or invalid, prints an error message:
 *
 *   Pi> peek
 *   error: peek requires 1 argument [address]
 *   Pi> peek fred
 *   error: peek cannot convert 'fred'
 *
 * Ignores any arguments after the first. Returns 0 on success, 1 on error.
 */
int cmd_peek(int argc, const char* argv[]);

/*
 * cmd_poke
 * ========
 *
 * Usage: poke [adress] [value]
 *
 * Stores `value` into the memory at `address`.
 * If a numeric argument is prefixed with 0x, it is interpreted as hex, 
 * otherwise interpreted as decimal. Address must be 4-byte aligned.
 *
 * Example:
 *
 *   Pi> poke 0x1000 3
 *   Pi> peek 0x1000
 *   0x00001000: 00000003
 *
 * If argument is missing or invalid, prints an error message:
 *
 *   Pi> poke 0xFFFC
 *   error: poke requires 2 arguments [address] and [value]
 *   Pi> poke bob 3
 *   error: poke cannot convert 'bob'
 *   Pi> poke 11 0
 *   error: poke address must be 4-byte aligned
 *
 * Ignores any arguments after the second. Returns 0 on success, 1 on error.
 */
int cmd_poke(int argc, const char* argv[]);


#endif
