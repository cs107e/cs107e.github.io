#ifndef TIMER_H
#define TIMER_H

/*
 * Hardware abstractions for a Raspberry Pi timer.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 *         Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Date: Jan 24, 2016
 * Edited by Mark Miller, Anna Zeng, Jan 21, 2018
 */

/*
 * Initialize the timer. For assignment 2, this does nothing.
 * However, all libpi peripheral modules require an init, so it is
 * included for consistency's sake.
 */
void timer_init(void);

/*
 * Returns the current system tick count. The tick count is set to
 * zero at boot and increments once per microsecond until the
 * processor resets.  One tick is equal to one microsecond.
 *
 * @return system tick count
 */
unsigned int timer_get_ticks(void);

/*
 * A simple busy loop that delays the program for `usec` microseconds.
 *
 * @param usec the number of microseconds to busy loop for
 */
void timer_delay_us(unsigned int usec);

/*
 * Delay for `msec` milliseconds.
 *
 * @param msec the number of milliseconds to busy loop for
 */
void timer_delay_ms(unsigned int msec);

/*
 * Delay for `sec` seconds.
 *
 * @param sec the number of seconds to busy loop for
 */
void timer_delay(unsigned int sec);

/*
 * Copyright (c) 2018 Stanford University.
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

#endif
