#include "malloc.h"
#include "printf.h"
#include "strings.h"
#include "uart.h"

void strings(void)
{
    char a[10];
    char b[] = "dopey";
    const char *c = "happy";
    char *d = malloc(10);
    memcpy(d, "grumpy", 7);

    // which of these memory locations are valid to write?
    *a = 'A';
    *b = 'B';
    *(char *)c = 'C'; // why typecast here?
    *d = 'D';

    printf("a = '%s' \n", a);
    printf("b = '%s' \n", b);
    printf("c = '%s' \n", c);
    printf("d = '%s' \n", d);

    printf("I'm %s!\n", "happy");
}

int main(void)
{
    printf("\nStarting program %s\n", __FILE__);
    strings();
    printf("\nProgram %s completed.\n\04", __FILE__);
    return 0;
}
