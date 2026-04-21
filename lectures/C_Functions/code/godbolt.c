int sum(int a, int b) {
    return a + b;
}

int twice(int arg) {
    return sum(arg, arg);
}

int binky(void) {
    return twice(9) + sum(5, 7);
}

void increment(int *arg) {
    (*arg)++;
}

int winky(void) {
    int var = 27;
    increment(&var);
    return twice(var + 3);
}
