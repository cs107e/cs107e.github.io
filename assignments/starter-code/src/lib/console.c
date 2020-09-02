#include "console.h"
#include <stdarg.h>

static void process_char(char ch);

void console_init(unsigned int nrows, unsigned int ncols)
{
    // TODO: implement this function
    // call `process_char` to silence the compiler's warning
    // that it's defined but not used
    // once you use `process_char` elsewhere, you can delete
    // this line of code
    process_char('a');
}

void console_clear(void)
{
    // TODO: implement this function
}

int console_printf(const char *format, ...)
{
    // TODO: implement this function, be sure to use your vsnprintf!
	return 0;
}

static void process_char(char ch)
{
    // TODO: implement this helper function (recommended)
    // if ordinary char: inserts ch into contents at current position
    // of cursor, cursor advances one position
    // if special char: (\r \n \f \b) handle according to specific function
}
