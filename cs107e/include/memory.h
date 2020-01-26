#ifndef MEMORY_H
#define MEMORY_H

/*
 * Macros for explicit memory reads and writes.
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 *
 * Date: Jan 24, 2016
 */
unsigned GET32(unsigned addr);
void PUT32(unsigned addr, unsigned word);

unsigned GET16(unsigned addr);
void PUT16(unsigned addr, unsigned word);

unsigned GET8(unsigned addr);
void PUT8(unsigned addr, unsigned word);

#endif
