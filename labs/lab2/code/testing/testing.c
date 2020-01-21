// File: testing.c
// ---------------
// Unit testing example

#include "assert.h"

// The count_bits function is intended to return the count of
// on bits in a numeric value. But oops-- the code as 
// written below is buggy!  A program this simple we could try to
// validate "by inspection" but instead we will demonstrate
// how to find the problem using rudimentary unit tests and
// the assert() function.
// You assert a condition that must be true and if the expression
// evaluates to false, assert treats as an error.
// We use the red and greeen LEDs on the Pi board as status indicators. 
// When an assert fails, it stops executing the program
// and flashes the red LED. If the program successfully finishes
// executing (no assert failed), the green LED turns on.
// Thus the flashing red LED of doom tells you that your
// program is failing at least one test.
// The solid green light means your program passed the tests you
// asked it to check.

int count_bits(unsigned int val)
{
    int count = 0;

    while (val != 0) {
        val = val >> 1;   // BUGGY: should shift AFTER testing low bit 
        if (val & 1)      // which inputs are affected by this bug?
            count++;
    }
    return count;
}


void main(void) 
{
    assert(count_bits(0) == 0);
    assert(count_bits(8) == 1);
    assert(count_bits(6) == 2);
    assert(count_bits(7) == 3);
    assert(count_bits(0xffffffff) == 32);
    assert(count_bits(5) == 1);   // what happens if my test case is misconstructed?

	// read cstart.c to find out what happens after main() finishes
}
