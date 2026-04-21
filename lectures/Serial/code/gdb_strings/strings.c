#include <stddef.h>

size_t my_strlen(const char *str) {
    int n = 0;
    while (str[n] != '\0') {
        n++;
    }
    return n;
}

int main(void)
{
    char arr[] = {'a', 'p', 'p', 'l', 'e','\0'};
    char arr2[] = "pear";     // short cut
    const char *ptr = "orange";
    char ch = 'c';
    char *cptr = &ch;

    int a = my_strlen("cherry");
    int b = my_strlen(arr);
    int c = my_strlen(arr2);
    int d = my_strlen(ptr);
    int e = my_strlen(cptr);
    int f = my_strlen(NULL);
    return a + b + c + d + e + f;
}
