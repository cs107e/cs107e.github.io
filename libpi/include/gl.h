#ifndef GL_H
#define GL_H

/* Functions for a simple bare metal Raspberry Pi graphics
 * library. Builds on the video library video.[ch] for frame buffer
 * access and configuration; trying to use both simultaneously
 * is discouraged.
 *
 * This is a simple double-buffering library. You can draw on a
 * writeable buffer with gl_block and other commands, then swap it in
 * as the display buffer with gl_draw.  The function gl_clear clears
 * the writeable buffer.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: August 14 2014
 */

#include "fb.h"

#define GL_SINGLEBUFFER FB_SINGLEBUFFER
#define GL_DOUBLEBUFFER FB_DOUBLEBUFFER

void gl_init(unsigned width, unsigned height, unsigned mode);

#define GL_BLACK 0xFF000000
#define GL_WHITE 0xFFFFFFFF
#define GL_RED   0xFF0000FF
#define GL_GREEN 0xFF00FF00
#define GL_BLUE  0xFFFF0000

color gl_color( unsigned char r, unsigned char g, unsigned char b );

unsigned gl_get_width(void);
unsigned gl_get_height(void);

void gl_clear(color c);

void gl_draw_pixel(int x, int y, color c);

void gl_draw_char(int x, int y, char letter, color c);

void gl_draw_string( int x, int y, char* string, color c);

unsigned gl_get_char_height(void);
unsigned gl_get_char_width(void);

void gl_swap_buffer(void);

#endif
