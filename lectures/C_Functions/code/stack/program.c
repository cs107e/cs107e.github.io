/*
 * Sample code to use as gdb practice.
 */

int add(int x, int y) {
    return x + y;
}

int twice(int num) {
    return add(num, num);
}

int sum(int *arr, int n) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        result += arr[i];
    }
    return result;
}

int make_array(void) {
    int arr[5];
    return sum(arr, 5);
}

int factorial(int n) {
    if (n == 1) return 1;
    else return n * factorial(n-1);
}

int main(void) {
    int a = add(5, 7);
    a = twice(a);
    int b = make_array();
    int c = factorial(5);
    return a + b + c;
}
