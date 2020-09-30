// File: codegen.c
// ---------------
// C examples that demo some interesting properties/transformations
// when C compiler generates ARM instructions.

// Note: some passages set a variable from an (uninitialized) global.
// If instead we had init to a constant, gcc is so smart that
// it will optimize away the rest of the work, so this
// is here to trick the compiler to treat as unknown. 

int global;

// Part (a): arithmetic
// Study each of the C statements and the translation to ARM assembly.
// line 1: ARM has no negate instruction -- what is used instead?
// line 2: What is used in place of multiply for this expression?
// line 3: Note that some parts of the complex arithmetic expression are
// pre-computed during compilation, and others are not.  Why the difference?
// Change the initialization of num to a constant value, e.g. int num = 3. 
// Now how much of the expression is the compiler able to precompute?
void part_a(void)
{
    int num = global;

    num = -num;
    num = num*10;
    num =  (num & ~(2 << 3)) * (((12 - 15)/4) + num); 
    global = num;
}

// Part (b): if
// An if statement can be structured as two distinct sequences of
// assembly instructions and branch used to route to the correct path.
// However, if the body of the conditional is short enough, the compiler 
// may choose to instead generate one unified sequence of conditionally 
// executed instructions. (This avoids use of somewhat expensive branch 
// instruction). Try the code below as-is to see the structure of the 
// unified sequence. Then edit the code to add more statements inside 
// the body until you find out how long the sequence must be for the
// the compiler to separate into a distinct branch.
void part_b(void)
{
    int num = global;

    if (num < 5) {
        num++;
    } 
    global = num;
} 

// Part (c): loops
// This function is a C version of the delay loop we used in
// the blink program.  If set to optimize at level -Og, the compiler's 
// generated assembly is a close match to our hand-written assembly. 
// Change the level to -O2 (aggressive optimization) -- woah! what 
// changed in the generated assembly? Why did that happen?
// Add volatile qualifier to the declaration of variable delay. 
// How does the generated assembly change now?
void part_c(void)
{
    int delay = 0x3f00;
    while (--delay != 0) ;
}

// Part (d): pointers
// The function below accesses memory using pointer/array
// notation. The code is divided into pairs of statements
// which perform a similar operation, but have subtle differences 
// (such as adding a typecast or operating on pointer of 
// different pointee type). How do those differences affect
// the generated assembly? 
void part_d(void)
{
    int *ptr = &global;
    char *cptr = (char *)ptr;
    int n = global;

    *ptr = 107;      // compare to next line
    *(char *)ptr = 107;

    ptr[5] = 13;     // compare to next line
    cptr[5] = 13;

    n = *(ptr + n);  // compare to next line
    n = ptr[n];

    global = n;
}


void main(void)
{
    // make calls to each function so none eliminated as dead code/unreachable
    part_a();
    part_b();
    part_c();
    part_d();
}

