int result;

int gauss(int n)
{
    int result = 0;
    for (int i = 0; i < n; i++)
        result += i;
    return result;
}

int sum(int x, int y)
{
    return x + y;
}

int abs(int v)
{
    return v < 0 ? -v : v;
}

int delta(int a, int b)
{
    return abs(a - b);
}

void main(void)
{
    int age = 19, course = 107;

    age = gauss(age + 3);
    result = sum(age, course) + delta(age, course);
}
