#include "printf.h"
#include "uart.h"

int one;
int two_init = 2;
const int three_const_init = 3;

static int four_static;
static int five_static_init = 3;
static const int six_static_const;
static const int seven_static_const_init = 4;

//const int eight_array[5];  // uncommment this line to add array symbol

static int use_variables(int a, int b)
{
    int local = 9;

    one = a;
    two_init = b + local;
    four_static += two_init;
    five_static_init += a;

    return three_const_init + six_static_const + seven_static_const_init;
}

int sum(int x, int y)
{
    return use_variables(x, y);
}

int main(void)
{
    uart_init();
    printf("Hello, world!\n");
    return sum(5, 10);
}
