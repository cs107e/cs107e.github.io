/*
 * Assembly routine to count leading zero bits
 * Author: Philip Levis
 */

.global count_leading_zeroes
count_leading_zeroes:
    clz r0, r0
    bx lr
