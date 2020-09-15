#ifndef SHELL_REF
#define SHELL_REF
#include "shell.h"
void ref_shell_init(formatted_fn_t print_fn);
void ref_shell_bell(void);
void ref_shell_readline(char buf[], size_t bufsize);
int ref_shell_evaluate(const char *line);
void ref_shell_run(void);
#endif
