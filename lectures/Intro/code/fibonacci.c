#include<stdio.h>
#include<stdlib.h>

int main() {
    int n = 10; // number of fibonacci terms
    int f0 = 0; // fibonacci number (1st term)
    int f1 = 1; // fibonacci number (2nd term)
    while (n > 0) {
        printf("%d\n", f0); // print the current term
        int nth = f0 + f1; // the nth term
        f0 = f1;  // update first
        f1 = nth; // update second
        n--; // decrement number of terms left
    }
    return 0;
}
