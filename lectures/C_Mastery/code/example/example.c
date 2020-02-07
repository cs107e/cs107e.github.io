#include "malloc.h"
#include "printf.h"
#include <stdint.h>
#include "strings.h"
#include "uart.h"

bool isnum(char ch)
{
    return ch >= '0' && ch <= '9';
}

bool isvowel(char ch)
{
    return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u';
}

void censor(char *s, bool (*pred)(char))
{
    for (int i = 0; s[i]; i++)
        if (pred(s[i])) s[i] = '-';
}

void function_ptr(void)
{
    char s[] = "CS107e";
    char t[] = "rox my sox";

    censor(s, isnum);
    printf("After censoring numbers: %s\n", s);
    censor(t, isvowel);
    printf("After censoring vowels: %s\n", t);
}


void strings(void) 
{
    char *a;
    char b[10];
    char *c = "happy";
    char d[] = "dopey";
    char e[] = {'g','r','u','m','p','y', '\0'};
    char *f = NULL;
    char *all[] = {a, b, c};
    char matrix[2][10];

    // write 'z', which of these memory locations are valid?
    a[0] = 'z';
    b[0] = 'z'; 
    c[0] = 'z';
    d[0] = 'z';
    e[0] = 'z';
    f[0] = 'z';
    all[1][1] = 'z';
    matrix[1][1] = 'z';

    printf("a='%s' b='%s' c='%s' d='%s' e='%s' f='%s'\n", a, b, c, d, e, f);
    printf("Are you %s now?\n", "happy");
    for (int i = 0; i < sizeof(all)/sizeof(*all); i++)
        printf("all[%d] = '%s'\n", i, all[i]);
    for (int i = 0; i < sizeof(matrix)/sizeof(*matrix); i++)
        printf("matrix[%d] = '%s'\n", i, matrix[i]);

}


int main(void)
{
    printf("\nStarting program %s\n", __FILE__);
    strings();  
    function_ptr();
    printf("\nProgram %s completed.\n\04", __FILE__);
    return 0;
}
