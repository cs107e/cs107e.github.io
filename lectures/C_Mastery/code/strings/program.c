#ifdef __riscv
#include "malloc.h"
#include "printf.h"
#include "strings.h"
#include "uart.h"
#else
#include "native.h"
#endif

void strings(void) {
    char a[10];
    char b[] = "dopey";
    const char *c = "happy";
    char *d = malloc(10);
    memcpy(d, "grumpy", 6);

    // which of these memory locations are valid to write?
    *a = 'A';
    *b = 'B';
    *(char *)c = 'C'; // why typecast here?
    *d = 'D';

    printf("a = %p '%s' \n", a, a);
    printf("b = %p '%s' \n", b, b);
    printf("c = %p '%s' \n", c, c);
    printf("d = %p '%s' \n", d, d);

    printf("I'm %p %s!\n", "happy", "happy");
}

int main(void) {
    uart_init();
    printf("\nStarting program %s\n", __FILE__);
    strings();
    printf("\nProgram %s completed.\n", __FILE__);
    return 0;
}
