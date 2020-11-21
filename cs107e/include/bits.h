#ifndef BITS_H
#define BITS_H

/*
 * Header file for bit counting routines in assembly code.
 *
 * You implement this module in assignment 7.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 *
 * Last updated:   February 2020
 */

/*
 * Returns the number of leading zeroes in val. Returns 32 if
 * no bits are set. 0x80000000 will return 0, while 0x1 will
 * return 31.
 */
unsigned int count_leading_zeroes(unsigned int val);

#endif
