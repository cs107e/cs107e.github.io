// Variety of example functions (leaf/not, locals/not, excess params/not, etc.)
// Examine generated asm to see how each is implemented

void copy(int *p, int *q)
{
    *p = *q;
}

int binky(int x, int y)
{
    return x + y;
}

int winky(int x)
{
    return binky(x, 87);
}

int dinky(void)
{
    return winky(5) + winky(22);
}

int pinky(int x)
{
    return winky(x) + winky(7) + x;
}

int megaparam(int a, int b, int c, 
              int d, int e, int f) {
    return b+d;
}

void use_address(int a) {
	int b = 10;
	copy(&a, &b);
}

int megalocal(void) {
    int arr[100];
    int n = sizeof(arr)/sizeof(*arr);
    copy(arr, &arr[1]);
    return arr[n-1];
}

int main(void)
{
    int x = winky(10);
    int y = megaparam(0, 10, 20, 30, 40, 50);
    return x + y;
}

