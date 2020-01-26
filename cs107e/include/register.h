#ifndef REGISTER_H
#define REGISTER_H


/*
 * Hardware abstractions for the ARM processor state registers
 * (CPSR and SPSR).
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Date; Mar 20, 2016
 */

#define CPSR_N (1<<31)
#define CPSR_Z (1<<30)
#define CPSR_C (1<<29)
#define CPSR_V (1<<28)

void SETCPSR(unsigned cpsr);
unsigned GETCPSR(void);

void SETSPSR(unsigned spsr);
unsigned GETSPSR(void);

#endif
