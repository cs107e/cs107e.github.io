#include <string.h>
#include <stdio.h>

int main()
{
    char *s = "hello, world\n";
    puts(s);
    s[5] = '\0';
    puts(s);
}
