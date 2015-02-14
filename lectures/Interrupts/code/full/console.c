/*
 * This application uses the interrupt-driven keyboard
 * driver to provide a console.
 *
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
*/

#include "system.h"
#include "gfx.h"
#include "kdriver.h"
#include "led.h"
#include "font.h"

#define ROWS 50
#define COLS 100
// Text is Y, X, so row, column
char text[ROWS][COLS];

int column;
int row;
//------------------------------------------------------------------------
int notmain(void) {
  enable_caches();
  enable_branch_prediction();
  kdriver_init();
  gfx_init();

  column = 0;
  row = 0;
  int color = gfx_compute_color(0xFF, 0xFF, 0xFF);
  int redraw = 0;
  while (1) {
    //led_toggle();
    while (kdriver_has_chars()) {
      char ch = kdriver_read_char();
      led_toggle();
      if (ch == '\b') {
        if (column != 0) {
          column--;
          text[row][column] = '\0';
        }
      }
      if (ch == '\n' || column >= COLS) {
        row++;
        column = 0;
        if (row >= ROWS) {
          row = 0;
        }
        text[row][column] = '\0';
      } 
      if (ch != '\n' && ch != '\b') {
        text[row][column] = ch;
        column++;
        text[row][column] = '\0';
      }
      redraw = 1;
    }
    if (redraw) {
      gfx_clear();
      int i;
      for (i = 0; i < ROWS; i++) {
        gfx_draw_string(color, 0, i * font_height(), text[i]);      
      }
      gfx_draw();
      redraw = 0;
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
