/*
 * A nonsense program that only exists to give you a chance
 * to explore relationships between arrays/pointers and
 * experiment with different ways to access memory.
 */

void pointer(int a[], int *b)
{
    b[2] = 9;
    a[2] *= 10;
}

int notmain(void)
{
    int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *ptr = arr;

    pointer(arr, ptr);

    return arr[2];
}
