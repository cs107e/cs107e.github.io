#include "printf.h"
#include "uart.h"

int sum(int n) {
    int result = 0;

    for (int i = 1; i <= n; i++)
        result += i;
    return result;
}

int factorial(int n) {
    if (n == 0) return 1;
    return n * factorial(n-1);
}

void main(void)
{
    uart_init();
    int val = 10;
    int result = sum(val);
    printf("Gaussian sum(%d) = %d\n", val, result);
    val = 7;
    result = factorial(val);
    printf("factorial(%d) = %d\n", val, result);
}
