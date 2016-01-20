// File: codegen.c
// ---------------
// C examples that demo some interesting properties/transformations
// when C compiler generates ARM instructions


// preprocessor define
#define DNUM 5

// global variable
int gnum = 5;

// global constant
const int cnum = 5;

// Part (a): constant-folding
// In the initialization of val below, replace DNUM with gnum or cnum and
// recompile to see what changes in the generated ARM when using a
// a global variable versus global constant versus a preprocessor #define.
// Note that none of the generated ARM contains a function call or any
// instructions to compute sizeof at runtime, a raw 4 is subtracted.
// sizeof is an entirely compile-time operator!
int constant_fold()
{
    int val = gnum; // replace DNUM with gnum or cnum, does generated ARM change?
    return (val*7 + 13)*val - sizeof(int);
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
// The is_prime function is buggy due to a missing initialization and furthermore
// it uses integer division/mod (not available in the ARM instruction set!). 
// Surprisingly, the code as-is builds without error, executes without incident, 
// and always returns false. Let's look at the ARM to figure out why.
// The local variable has_factor is intended to track whether a divisor has
// been found. The value of a local is unreliable until set. The oh-so-clever
// compiler realizes that loop might set the value to 1 but otherwise, its
// value is left indeterminate. Use of an uninitialized value is an undefined
// behavior for C, so the compiler has latitude to handle in any way it chooses.
// In this case, the compiler chooses to pretend that the unset value would
// be a 1. This conveniently means that the value will be 1 before,
// during, and after the loop, no matter what happens in the loop at all!
// The function always returns 0; the compiler can eliminate the rest of the
// function body as it has no effect whatsoever. Wow -- what an optimization!
// Correct the code by making an initial assignment of has_factor to 0. Now
// try to rebuild. Ooops! The build now fails due to a link error. The compiler
// has generated division as a call to a library routine (__aeabi_uidivmod)
// that is not available in our bare metal world. We'll talk about the linker
// in a future lecture.
 __attribute__((optimize("-O2")))    // GCC directive to ask for level 2 optimization
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
// using an ordinary for loop. The C language semantics dictate that
// both for and while loops are pre-tested (i.e. the test expression 
// is evaluated before the first and every subsequent iteration). First
// sketch your own arrangement of ARM instructions to match the control
// flow for a C for-loop (i.e. init, followed by test, body, incr).
// Now look at the generated ARM to see how the compiler arranged it.
// It may help to print out the listing and annotate which section
// corresponds to init/test/body/incr. The compiler's arrangement
// makes a special case out of the first iteration so as to streamline
// each subsequent loop iteration to need only one branch, not two. I
// In many architectures (ARM included), branch instructions are more
// costly than others, so the compiler is eager to minimize their use.
int discrete_log(int n)
{
    for (int pow = 31; pow > 0; pow--) {
        if (n & (1 << pow)) return pow;
    }
    return 0;
}

// More part (d): loops and unrolling
// This is a copy of the same function as above, but with an added GCC
// directive to enable an aggressive loop optimization known as
// "loop unrolling". Look at the generated ARM for this function and
// compare to the original above. Can you identify what kind of
// transformation is made by loop unrolling?
 __attribute__((optimize("-funroll-loops"))) // GCC-specific directive
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
