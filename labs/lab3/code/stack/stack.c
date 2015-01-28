int A(int a, int b)
{
    return a-b;
}

int B(int a, int b, int c)
{
    int d = a - b;
    return d - A(c,d);
}

int notmain(void)
{
    return B(1,2,3);
}

