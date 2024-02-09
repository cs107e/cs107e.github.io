#ifdef __riscv
#include "printf.h"
#include "uart.h"
#include "strings.h"
#else
#include "native.h"
#endif


static int make_trouble(int how_much) {
    char buf[5] = "abcd";
    printf("How much trouble? Writing %d bytes beyond... ", how_much);
    memset(&buf[4], 0, how_much);
    return how_much;
}

int main(void) {
    uart_init();
    static int count;

    printf("\n\n Starting %s \n\n", __FILE__);

    for (count = 1; count < 100; count++) {
        if (make_trouble(count))
            printf("success!\n");
    }
    printf("\n\n Completed main() of %s \n", __FILE__);
    return 0;
}
