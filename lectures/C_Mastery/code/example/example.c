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
    char t[] = "Segmentation Fault";

    censor(s, isnum);
    printf("After censoring numbers: %s\n", s);
    censor(t, isvowel);
    printf("After censoring vowels: %s\n", t);
}


void strings(void) 
{
    char *a;
    char *b = "happy";
    char c[30];
    char d[] = "dopey";
    char *e = NULL;
    char *all[] = {a, b, c, d, e};   

    a[0] = 'z';
    b[0] = 'z';
    c[0] = 'z';
    d[0] = 'z';
    e[0] = 'z';
    printf("a='%s' b='%s' c='%s' d='%s'\n", a, b, c, d);
    printf("Are you %s now?\n", "happy");
    for (int i = 0; i < sizeof(all)/sizeof(*all); i++)
        printf("all[%d] = '%s'\n", i, all[i]);
}


int main(void)
{
    function_ptr();
    strings();  
    printf("\nProgram %s completed.\n\04", __FILE__);
    return 0;
}
