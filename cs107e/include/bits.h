#ifndef BITS_H
#define BITS_H

/*
 * Header file for bit counting routines in assembly code.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 *
 * Last updated:   February 2020
 */

/*
 * `count_leading_zeroes`
 *
 * Count the number of leading zero bits in `val`. Counting
 * proceeds from most significant to least significant bit.
 * Examples:
 *    count_leading_zeroes(0) => 32
 *    count_leading_zeroes(0x80000000 ) => 0
 *    count_leading_zeroes(0x1) => 31
 *
 * @param val   32-bit unsigned value
 * @return      the count of leading zeroes in val
 */
unsigned int count_leading_zeroes(unsigned int val);

#endif
