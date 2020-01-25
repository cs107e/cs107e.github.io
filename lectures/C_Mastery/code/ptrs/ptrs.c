#include "malloc.h"
#include "printf.h"
#include <stdint.h>
#include "strings.h"

char *strdup(char *s)
{
    int len = strlen(s);
    char *copy = malloc(len+1);
    memcpy(copy, s, len+1);
    return copy;
}

int isdigit(char ch)
{
    return ch >= '0' && ch <= '9';
}

void censor(char *s, int (*pred)(char))
{
    for (int i = 0; s[i]; i++)
        if (pred(s[i])) s[i] = '-';
}

void function_ptr(void)
{
    char *a = strdup("cs107e");

    censor(a, isdigit);
    printf("After censoring: %s\n", a);
}

struct item {
    char name[16];
    int sku;
    int price;
};

void struct_ptr(void)
{
    struct item it;
    struct item *p;

    p = &it;
    it.sku = 10;
    p->sku = 20;
    printf("Struct = %d Ptr = %d \n", it.sku, p->sku);
}


void strings(void) 
{
    char *a;
    char *b = "happy";
    char c[30];
    char d[] = "dopey";
    char *dwarves[] = {a, b, c, d};   

    a[0] = 'z';
    b[0] = 'z';
    c[0] = 'z';
    d[0] = 'z';
    printf("a='%s' b='%s' c='%s' d='%s'\n", a, b, c, d);
    printf("Are you %s now?\n", "happy");
    for (int i = 0; i < sizeof(dwarves)/sizeof(*dwarves); i++)
        printf("dwarves[%d] = '%s'\n", i, dwarves[i]);
}


int main(void)
{
    strings();  
    struct_ptr();
    function_ptr();
    printf("\nProgram %s completed.\n\04", __FILE__);
    return 0;
}
