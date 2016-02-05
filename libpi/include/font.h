#ifndef FONT_H
#define FONT_H

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
int font_get_height();

/* The pixel width of a character. */
int font_get_width();

/* The depth (in bytes) of a character pixel. */
int font_get_depth();

/* The number of bytes needed to store a character
   image. This is equal to height * width * depth. */
int font_get_size();

/* Fill in the image of character `ch` into the buffer
   pointed by `buf`. Returns 0 on failure and 1 on success.
   Failure is when `buflen` does not equal the value
   returned by font_buflen(), used as a basic sanity
   check. After this function returns, buf is a width*height
   image of the pixel, i.e., a char[height][width][depth]
   array. */
int font_get_char(char ch, char* buf, int buflen);

#endif
