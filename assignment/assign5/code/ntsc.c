/*
 * This application displays the NSTC test pattern.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
*/

#include "fb.h"
#include "led.h"

// The top 7 bars: the last bar is a little wide, so it
// has two entries in the 8 entry array.
static unsigned int bars[] = {0xFFC0C0C0,
                              0xFF00C0C0,
                              0xFFC0C000,
                              0xFF00C000,
                              0xFFC000C0,
                              0xFF0000C0,
                              0XFFC00000,
                              0XFFC00000};

// The 7 castellations in the middle. The last bar is a little
// wide, so it has two entries in the 8 entry array.
static unsigned int castellations[] = {0xFFC00000,
                                       0xFF131313,
                                       0xFFC000C0,
                                       0xFF131313,
                                       0xFFC0C000,
                                       0xFF131313,
                                       0xFFC0C0C0,
                                       0xFFC0C0C0};
    

//------------------------------------------------------------------------
void notmain(void) { 
  fb_init();

  if (fb_height() != 480 ||
      fb_width() != 640 ||
      fb_byte_depth() != 4) {
    return;
  }
  
  unsigned int* rb = (unsigned int*)fb_writeable_buffer();
  unsigned int rx, ry;

  unsigned int (*fb)[640] = (unsigned int(*)[640])rb;
  unsigned int len = fb_height() * fb_width();
  int i;

  for (ry = 0; ry < 480; ry++) {
    if (ry < 360) {  // Top bars are 0-359
      for (rx = 0; rx < 640; rx++) {
        unsigned int index = rx / 90;
        fb[ry][rx] = bars[index];
      }
    } else if (ry < 384 ) { // Castellations are 360-383
      for (rx = 0; rx < 640; rx++) {
        unsigned int index = rx / 90;
         fb[ry][rx] = castellations[index];
      }
    } else { // Bottom pattern is 384-480
      // The bottom pattern has a different geometry so just
      // brute force it with the X offsets.
      for (rx = 0; rx < 640; rx++) {
        if (rx < 112) {
          fb[ry][rx] = 0xFF4A2100; // -I
        } else if (rx < 224) {
          fb[ry][rx] = 0xFFFFFFFF; // white
        } else if (rx < 336) {
          fb[ry][rx] = 0xFF6A0032; // Q
        } else if (rx < 450) {
          fb[ry][rx] = 0xFF131313; // Black
        } else if (rx < 480) {
          fb[ry][rx] = 0xFF000000; // -4
        } else if (rx < 510) {
          fb[ry][rx] = 0xFF131313; // 0
        } else if (rx < 540) {
          fb[ry][rx] = 0xFF262626; // +4
        } else if (rx < 640) {
          fb[ry][rx] = 0xFF131313; // Black
        }
      }
    }
  }

  while (1) {}
  return;
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
