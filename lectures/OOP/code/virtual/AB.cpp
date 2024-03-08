#include "AB.h"

int Base::f(int c) {
    return x+y+c;
}

int A::f(int c) {
    return 2*x+y+c;
}

int B::f(int c) {
    return x+2*y+c;
}
