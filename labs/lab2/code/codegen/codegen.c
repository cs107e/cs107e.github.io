// File: codegen.c
// ---------------
// C examples that demo some interesting properties/transformations
// when C compiler generates ARM instructions


// preprocessor define
#define DNUM 4

// global variable
int gnum = 4;

// global constant
const int cnum = 4;

// Part (a): constant-folding
// In the initiaization of val below, replace DNUM with gnum or cnum and
// recompile to see the generated ARM.  What changes when using a
// a global variable or constants versus a preprocessor #define?
int constant_fold()
{
    int val = DNUM; // replace DNUM with gnum or cnum, does generated ARM change?
    return (val*7 + 13)*2 - 1;
}


// Part (b): if/else versus switch
// The pair of functions below choose a path from a set of options by
// comparing to compile-time constants. Both functions make same 
// computation, but one is expressed as if-else, the other as a switch.
// Look at the generated ARM to see that both functions are using 
// effectively the same instructions, despite the fairly different
// expression in C. Interesting!
int choice_if(int n)
{
    if (n == 5) {
        return 99;
    } else if (n == 13) {
        return 197;
    } else {
        return 0;
    }
}

int choice_switch(int n)
{
    switch(n) {
        case  5: return 99;
        case 13: return 197;
        default: return 0;
    }
}

// More part (b): this pair of functions adds a few more options to the
// choices and the constants are grouped within a narrow range.
// Look at the generated ARM for these two functions to see how a
// different tactic is used for the switch this time.  This "jump table"
// is an optimization unique to a dense switch statement.
int dense_choice_if(int n)
{
    if (n == 100) {
        return 99;
    } else if (n == 102) {
        return 197;
    } else if (n == 105) {
        return 53;
    } else if (n == 107) {
        return 81;
    } else {
        return 0;
    }
}


int dense_choice_switch(int n)
{
    switch(n) {
        case 100: return 1;
        case 102: return 2;
        case 105: return 3;
        case 107: return 4;
//        case 500: return 5;  // uncomment this line, does generated ARM change?
        default:  return 0;
    }
}


// Part (c): undefined behavior, ARM divide
// The is_prime function is buggy due to a missing initialization. Furthermore
// it uses integer division/mod (not available in the ARM instruction set!). 
// Surprisingly, the code as-is builds without error, executes without incident, 
// and always returns false. Let's look at the ARM to figure out why.
// First consider the use of division to initialize i. Look at generated ARM 
// instructions for this statement. How was no division needed? Why was this 
// transformation legal?
// Now consider the mod in the loop body. (Mod is the dual to div, both require
// a division operation). The same trick that worked above doesn't apply here, 
// yet it compiles anyway. How was no division needed? Why was this 
// transformation legal?  
// Correct the bug in the code. What happens when you now try to compile?
int is_prime(int n)
{
    int has_factor;     // BUG: missing initialization
    for (int i = n/2; i > 1; i--) {
        if ((n % i) == 0) has_factor = 1;
    }
    return !has_factor;
}


// Part (d): loops
// This function computes the binary discrete logarithm of its argument 
// using an ordinary for loop. The C language semantics require that
// both for and while loops are pre-tested (i.e. the test expression 
// is evaluated before the first and every subsequent iteration). Sketch 
// for yourself how to arrange the ARM instructions to direct control flow
// to match a C for-loop (i.e. first init, followed by test, body, incr).
// Now look at the actual generated ARM and see how the compiler arranged it.
// What did it do with the test? Do you have any idea why it did that? 
int discrete_log(int n)
{
    for (int pow = 31; pow > 0; pow--) {
        if (n & (1 << pow)) return pow;
    }
    return 0;
}

// More part (d): loops and unrolling
// This is a copy of the same function as above, but wtih an added GCC
// derivate to enable an aggressive loop optimization known as 
// "loop unrolling". Look at the generated ARM for this function and
// compare to the original above. What transformation is being applied
// by loop unrolling?
 __attribute__((optimize("-funroll-loops"))) 
int discrete_log_alt(int n)
{
    for (int pow = 31; pow > 0; pow--) {
        if (n & (1 << pow)) return pow;
    }
    return 0;
}


void main()
{
    // make calls to each function so all are used/necessary for code gen
    gnum = constant_fold() + choice_if(107) + choice_switch(107) +
        dense_choice_if(107) + dense_choice_switch(107) + is_prime(107) + 
        discrete_log(107) + discrete_log_alt(107);
}
