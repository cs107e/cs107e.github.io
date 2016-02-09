#include <stdio.h>

int main(void)
{
    int a[2][2] = { {0, 1}, {2, 3}};;
    int  *b = &a[0][0];
    int (*c)[2] = a;
    int (*d)[2][2] = &a;

    printf("a = %08x\n", (unsigned)a );
    printf("&a[0][0] = %08x\n", (unsigned)(&a[0][0]) );
    printf("&a[0][1] = %08x\n", (unsigned)(&a[0][1]) );
    printf("b = %08x\n", (unsigned)b );
    printf("b+1 = %08x\n", (unsigned)(b+1) );
    printf("b+2 = %08x\n", (unsigned)(b+2) );
    printf("&a[0] = %08x\n", (unsigned)(&a[0]) );
    printf("&a[1] = %08x\n", (unsigned)(&a[1]) );
    printf("c = %08x\n", (unsigned)c );
    printf("c+1 = %08x\n", (unsigned)(c+1) );
    printf("d = %08x\n", (unsigned)d );
    printf("d+1 = %08x\n", (unsigned)(d+1) );

    int a1[] = { 0, 1 };
    int a2[] = { 2, 3, 4 }; 
    int *p[2] = { a1, a2 };
    int *q = p[0];

    printf("a1 = %08x\n", (unsigned)a1 );
    printf("a2 = %08x\n", (unsigned)a2 );
    printf("p = %08x\n", (unsigned)p );
    printf("q = %08x\n", (unsigned)q );
    printf("p[0] = %08x\n", (unsigned)p[0] );
    printf("p[1] = %08x\n", (unsigned)p[1] );

}
