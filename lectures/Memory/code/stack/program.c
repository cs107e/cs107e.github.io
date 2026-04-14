/* Add some global variables to see placement by linker */
int g_arr[] = {1, 3, 5, 7};
int g_blank[10];
const char *str = "Stanford University";


int sqr(int v) {
    return v * v;
}

int delta(int a, int b) {
    int diff = sqr(a) - sqr(b);
    return diff;
}

int main(void) {
    int y = delta(3, 7);

    return y * 2;
}
