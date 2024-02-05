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
