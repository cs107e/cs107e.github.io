/* Hardware abstraction functions for a Raspberry Pi timer.
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: August 14 2014
 */ 

#include "system.h"
#include "timer.h"

#define ARM_TIMER_LOD 0x2000B400
#define ARM_TIMER_VAL 0x2000B404
#define ARM_TIMER_CTL 0x2000B408
#define ARM_TIMER_DIV 0x2000B41C
#define ARM_TIMER_CNT 0x2000B420

static char has_fired_;
static unsigned int last_firing_;

/* The ARM timer is 250MHz; prescale by 250 to get
   a 1MHz clock. */
void timer_init() {
  has_fired_ = 0;
  last_firing_ = 0;
  // Disable free-running counter, set prescalar to 250 (F9)
  PUT32(ARM_TIMER_CTL,0x00F90000);
  // Re-enable free-running counter, set prescalar to 250 (F9)
  PUT32(ARM_TIMER_CTL,0x00F90200);  
}

unsigned int timer_gettime() {
  return GET32(ARM_TIMER_CNT);
}

void timer_wait_until(unsigned int time) {
  while (timer_gettime() != time) {
    // Do nothing;
  }
  has_fired_ = 1;
  last_firing_ = timer_gettime();
}

void timer_wait_for(unsigned int microseconds) {
  unsigned int current = timer_gettime();
  while ((timer_gettime() - current) < microseconds) {
    // do nothing
  }
  has_fired_ = 1;
  last_firing_ = timer_gettime();
}

void timer_wait_interval(unsigned int microseconds) {
  if (!has_fired_) {
    timer_wait_for(microseconds);
  } else {
    unsigned int stop = last_firing_ + microseconds;
    unsigned int remaining = stop - timer_gettime();
    // We've already passed the interval, just fire
    if (remaining > microseconds) {
      last_firing_ = timer_gettime();
      return;
    } else {
      timer_wait_for(remaining);
    }
  }
}

/*
 * Copyright (c) 2014 Stanford University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the Stanford University nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL STANFORD
 * UNIVERSITY OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
