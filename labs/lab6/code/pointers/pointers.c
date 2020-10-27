#include "printf.h"
#include "uart.h"

void main(void)
{
    uart_init();

    // Basic pointer review: What's the difference between incrementing a `char
    // *` and a `unsigned int *`?
    printf("\nPointer review:\n");
    printf("===============\n");
    char *char_ptr = "Hello";
    unsigned int uint_arr[] = {0, 1, 2};
    unsigned int *uint_ptr = uint_arr;

    printf("char_ptr   = %p\n", char_ptr);
    printf("char_ptr+1 = %p\n", char_ptr + 1);

    printf("uint_ptr   = %p\n", uint_ptr);
    printf("uint_ptr+1 = %p\n", uint_ptr + 1);

    // What happens if we walk off the end of the array?
    printf("\nWalking off the end of the array uint_arr: \n");
    for (int i = 0; i < 4; ++i) {
        printf("[%d] = %x ", i, uint_ptr[i]);
    }
    printf("\n");

    printf("\nSingle-dimensional arrays and pointers\n");
    printf("======================================\n");
    int f[] = { 0, 1 };
    int g[] = { 2, 3, 4 };
    // will the following declaration work?
    //int *h = {2, 3, 4};
    printf("f = %p\n", f);
    printf("g = %p\n", g);

    int *p[2] = { f, g };
    int *q = p[0];
    printf("p = %p\n", p);
    printf("q = %p\n", q);
    printf("p[0] = %p\n", p[0]);
    printf("p[1] = %p\n", p[1]);

    printf("\nPointers to pointers example\n");
    printf("==============================\n");
    const char *tokens[2] = { "Hello", "World" };
    printf("tokens   = %p \n", tokens);
    printf("tokens+1 = %p \n", tokens + 1);
    printf("tokens[0] = %p ('%s')\n", tokens[0], tokens[0]);
    printf("tokens[1] = %p ('%s')\n", tokens[1], tokens[1]);
    printf("*tokens[0] = %c\n", *tokens[0]);
    printf("*tokens[1] = %c\n", *tokens[1]);
    printf("**tokens = %c\n", **tokens);
    printf("**(tokens + 1) = %c\n", **(tokens + 1));
    printf("*(*tokens + 1) = %c\n", *(*tokens + 1));
    printf("**tokens + 1 = %c\n", **tokens + 1);

    printf("\nMulti-dimensional arrays and pointers\n");
    printf("=====================================\n");
    int a[2][2] = { {0, 1}, {2, 3} };
    int *b = &a[0][0];
    int (*c)[2] = a;
    int (*d)[2][2] = &a;

    printf("a = %p\n", a );
    printf("&a[0][0] = %p\n", &a[0][0] );
    printf("&a[0][1] = %p\n",  &a[0][1]);

    printf("b   = %p\n", b );
    printf("b+1 = %p\n", b+1);
    printf("b+2 = %p\n", b+2);
    printf("&a[0] = %p\n", &a[0]);
    printf("&a[1] = %p\n", &a[1]);
    printf("c   = %p\n", c );
    printf("c+1 = %p\n", c+1);
    printf("d   = %p\n", d );
    printf("d+1 = %p\n", d+1);

    uart_putchar(EOT);
}
