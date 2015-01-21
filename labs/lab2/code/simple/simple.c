/*
 * A set of simple C snippets that illustrate different optimizations 
 * compilers can do.
 *
 * You should disassemble and work out the mapping from C code to asm.
 */

/**********************************************************************/

int simple_mul_3(int x, int y) { return x * 3; }
int simple_mul_21(int x, int y) { return x * (2 * 1); }
int simple_div_3(int x, int y) { return x / 3; }
int simple_div(int x, int y) { return x / y; }

int deadcode(int x) {
    if(x == 0)
        return 1;
    else {
        if(x == 0) {
            foo();
            bar();
        }
        return 1;
    }
}

/**********************************************************************/

int foo() { return 3; }
int inlined(void) {
    return foo();
}

/**********************************************************************/
