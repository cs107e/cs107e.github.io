#ifndef SHELL_COMMANDS_REF
#define SHELL_COMMANDS_REF
#include "shell_commands.h"
int ref_cmd_echo(int argc, const char *argv[]);
int ref_cmd_help(int argc, const char *argv[]);
int ref_cmd_reboot(int argc, const char* argv[]);
int ref_cmd_peek(int argc, const char* argv[]);
int ref_cmd_poke(int argc, const char* argv[]);
#endif
