#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

/*
 * Header file for functions that provide greyscale bitmaps of
 * ASCII characters. Each character is 19 rows of 12 columns, the
 * bottom row is always 0. The pixels are 32-bits (RGBA).
 * The pixel map was generate by GIMP and is for the Linux
 * Libertine Mono font.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: December 18 2014
 */ 

/* The pixel height of a character. */
int font_height();

/* The pixel width of a character. */
int font_width();

/* The depth (in bytes) of a character pixel. */
int font_depth();

/* The number of bytes needed to store a character
   image. This is equal to height * width * depth. */
int font_buflen();

/* Fill in the image of character `ch` into the buffer
   pointed by `buf`. Returns 0 on failure and 1 on success.
   Failure is when `buflen` does not equal the value
   returned by font_buflen(), used as a basic sanity
   check. After this function returns, buf is a width*height
   image of the pixel, i.e., a char[height][width][depth]
   array. */
int font_ascii(char ch, char* buf, int buflen);
        
#endif

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
