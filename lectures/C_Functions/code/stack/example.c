#include "assert.h"

int rec_fact(int n)
{
    if (n == 0) {
        return 1;
    } else {
        return n*rec_fact(n-1);
    }
}

int factorial(int n)
{
    int result = 1;
    for (int i = n; i > 0; i--)
        result *= i;
    return result;
}

int sum(int x, int y, int z)
{
    return x + y + z;
}

int square(int v)
{
    return v * v;
}

int delta(int a, int b)
{
    int diff = square(a) - square(b);
    return diff * 2;
}

int main(void)
{
    int x = sum(5, 11, 8);
    int y = delta(3, x);

    int i = factorial(5);
    int r = rec_fact(5);
    assert(i == r);

    return y;
}
