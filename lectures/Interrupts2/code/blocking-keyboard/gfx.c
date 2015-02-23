/* Functions for a simple graphics library for the bare metal
 * Raspberry Pi.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: August 14 2014
 */ 

#include "gfx.h"
#include "fb.h"
#include "system.h"
#include "font.h"

void gfx_init() {
  fb_init();
}

unsigned int gfx_compute_color(unsigned char red,
                               unsigned char green,
                               unsigned char blue) {
  return (0xFF000000 | blue << 16 | green << 8 | red);
}

unsigned int gfx_get_width() {
  return fb_width();
}

unsigned int gfx_get_height() {
  return fb_height();
}

void gfx_plot(unsigned int color,
              unsigned int x,
              unsigned int y) {
  if (x >= fb_width() ||
      y >= fb_height()) {
    return;
  }
  unsigned int addr = (unsigned int)fb_writeable_buffer();
  addr += ((y * fb_width()) + x) * fb_byte_depth();
  PUT32(addr, color);
}

unsigned int gfx_letter_height() {
  return font_height();
}

unsigned int gfx_letter_width() {
  return font_width();
}

unsigned int gfx_draw_letter(unsigned int color,
                             unsigned int x,
                             unsigned int y,
                             char letter) {
  int bufsize = font_buflen();
  int row_width = font_width();
  if (letter < ' ' || letter > '~') {
    return 0;
  }
  else {
    unsigned int xindex, yindex;
    char buffer[bufsize];
    font_ascii(letter, buffer, bufsize);
    unsigned int (*pixels)[row_width] = (unsigned int (*)[row_width])buffer;
    for (yindex = 0; yindex < font_height(); yindex++) {
      for (xindex = 0; xindex < font_width(); xindex++) {
        unsigned int pixel_val = pixels[yindex][xindex];
        gfx_plot(pixel_val, x + xindex, y + yindex);
      }
    }
  }
  return 1;
}
  

unsigned int gfx_draw_string(unsigned int color,
                             unsigned int x,
                             unsigned int y,
                             char* str) {
  char* ptr;
  int xpos = x;
  for (ptr = str; *ptr != 0; ptr++) {
    if (gfx_draw_letter(color, xpos, y, *ptr)) {
      xpos += (gfx_letter_width() + 1);
    } else {
      return 0;
    }
  }
  return 1;
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
