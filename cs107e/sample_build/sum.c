int sum(int n) {
    int result = 0;

    for (int i = 1; i <= n; i++)
        result += i;
    return result;
}

int main(void)  {
    int a = sum(10);
    int b = sum(5);
    return a + b;
}
