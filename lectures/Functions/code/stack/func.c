int A(int a, int b)
{
    return a - b;
}

int B(int a, int b, int c)
{
    int d = a + 2;
    return d + A(b, c);
}


