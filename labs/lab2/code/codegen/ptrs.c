// File: ptrs.c
// ------------
// Simple C examples to show pointer/array access

int gnum = 107;

// Part (a): dead-code elimination
// This function tries to demo some various legal operations
// on pointers. Take a look at the generated ARM for this
// function to see that it is mostly empty! What gives?
// The optimizer compiler is quite aggressive about removing
// so-called "dead code" such as statements that are never
// reached/executed or code that has no effect (does not change
// any observable results). In this function, most of the
// statements read and write local variables and parameters, which
// are private to the function call and go away when the function
// completes. The only observable result of the function is
// the return value, whose code is properly generated.
int where_did_my_code_go(int num)
{
    int val, *p, *q;

    val = 10;
    p = &num;
    q = &val;
    *p = *q;        // only reads/writes local/param
    // The test expression below compares the memory location of a
    // local variable versus a parameter. The compiler knows that the
    // two addresses are never equal (two distinct variables!) so
    // will eliminate as dead code.
    if (&num == &val)
        gnum = 20;  // would change observable state, but never executed
    return gnum;
}


// Part (b): arrays and pointers
// Review the generated ARM for the array_ptr and main functions below
// Look carefully at the instructions used to access the nth
// array element and compare to the same computation expressed in C as
// pointer arithmetic.
int array_ptr(int arr[], int *ptr, int index)
{
    int sum = 0;

    sum += arr[5];      // what ARM is generated to access array elem at constant index?
    sum += arr[index];  // how about array elem at variable index?

    sum += *(ptr + 5);  // Does ptr addition generate same ARM as when use array syntax?
    sum += *(ptr + index);

    sum +=  *((char *)ptr + index);  // what changes when pointer typecast added?
    return sum;
}

void main()
{
    int arr[15], *ptr;
    int count = sizeof(arr)/sizeof(arr[0]); // what ARM is generated for this division?

    for (int i = 0; i < count; i++)
        arr[i] = i;

    ptr = arr;  // equivalently = &arr[0]
    array_ptr(arr, ptr, count/2);
}


// Part (c): GPIO pointers/bits
// Look at the generated ARM for these two functions and compare to
// the hand-written ARM from your larson scanner. Despite the C
// showing several uses of the div/mod operators, the generated ARM
// doesn't contain any references to the (non-existent) divide.
// Why not?
// For set_pin_to_output_mode, the "pin" argument is expected to
// be a number from 1 to 54. What would be the consequence if the
// function is called with a pin number outside the valid range,
// say -1 or 150? Might it be better for the function to ignore such
// an invalid request than do something wonky?
// Although such a call could be considered client error, anticipating
// that possibility and writing your code defensively can preempt future 
// heartache and headache!

#define GPIO_FSEL ((int*)0x20200000)
#define GPIO_SET  ((int*)0x2020001C)

void set_pin_to_output_mode(int pin)
{
     int index = pin/10;
     int pos = (pin%10) * 3;
     GPIO_FSEL[index] |= 1 << pos; // consider: why |= must be used here, not just =
}

void set_pin_on(int pin)
{
     int index = pin/32;
     int pos = pin%32;
     GPIO_SET[index] = 1 << pos;
}
