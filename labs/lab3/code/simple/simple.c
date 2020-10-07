/* 
 * Sample code to use as gdb practice.
 */

int abs(int v)
{
    int result = v < 0 ? -v : v;
    return result;
}

int factorial(int n)
{
    if (n <= 1)
        return 1;
    else
        return n*factorial(n-1);
}

int make_array(void)
{
    int array[6];

    array[0] = 1;
    for (int i = 0; i < 5; i++)
        array[i+1] = array[i] + 2;
    return abs(array[3]);
}

int diff(int a, int b)
{
    return abs(a - b);
}

int main(void)
{
    int x = 33, y = 107;

    int d = diff(x, y);
    int f = factorial(7);
    int n = make_array();
    return d + f + n;
}
