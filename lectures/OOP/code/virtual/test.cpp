#include "AB.h"

int call_f(Base *base, int c) {
    return base->f(c);
}

int main() {
    A a;
    int x = call_f(&a, 1);

    B b;
    int y = call_f(&b, 1);
}
