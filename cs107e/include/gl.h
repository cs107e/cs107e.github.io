#ifndef GL_H
#define GL_H

/*
 * Functions for a simple graphics library that draws
 * pixels, text, rectangles, lines, and triangles. Builds
 * on the fb module to configure and access the framebuffer.
 * Attempting to use both fb and gl simultaneously is discouraged.
 *
 * Students implement this module in assignment 6.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 */

#include "fb.h"
#include <stdint.h>

typedef enum {
    GL_SINGLEBUFFER = FB_SINGLEBUFFER,
    GL_DOUBLEBUFFER = FB_DOUBLEBUFFER }
gl_mode_t;

/*
 * `gl_init` : Required initialization for graphics library
 *
 * Initialize the graphic library. This function will call `fb_init` in turn
 * to initialize the framebuffer.
 *
 * @param width     requested framebuffer width (in pixels)
 * @param height    requested framebuffer height (in pixels)
 * @param mode      whether framebuffer should be
 *                  single buffered (GL_SINGLEBUFFER)
 *                  or double buffered (GL_DOUBLEBUFFER)
 *
 * A subsequent call to gl_init after the first does a re-initialization.
 */
void gl_init(int width, int height, gl_mode_t mode);

/*
 * `gl_get_width`
 *
 * Get the current width of the framebuffer.
 *
 * @return    the width (in pixels)
 */
int gl_get_width(void);

/*
 * `gl_get_height`
 *
 * Get the current height of the framebuffer.
 *
 * @return    the height (in pixels)
 */
int gl_get_height(void);

/*
 * `color_t`
 *
 * Define a type for color. We use BGRA colors, where each color
 * component R, B, G, or A is a single unsigned byte. The least
 * signficant byte is the B component, and A is most significant.
 */
typedef uint32_t color_t;

/*
 * Define some common colors ...
 *
 * Note that colors are BGRA, where B is the first byte in memory
 * and the least significant byte in the unsigned word.
 */
#define GL_BLACK    0xFF000000
#define GL_WHITE    0xFFFFFFFF
#define GL_RED      0xFFFF0000
#define GL_GREEN    0xFF00FF00
#define GL_BLUE     0xFF0000FF
#define GL_CYAN     0xFF00FFFF
#define GL_MAGENTA  0xFFFF00FF
#define GL_YELLOW   0xFFFFFF00
#define GL_AMBER    0xFFFFBF00
#define GL_ORANGE   0xFFFF3F00
#define GL_PURPLE   0xFF7F00FF
#define GL_INDIGO   0xFF000040
#define GL_CAYENNE  0xFF400000
#define GL_MOSS     0xFF004000
#define GL_SILVER   0xFFBBBBBB

/*
 * `gl_color`
*
 * Returns a color composed of the specified red, green, and
 * blue components. The alpha component of the color will be
 * set to 0xff (fully opaque).
 *
 * @param r     red component of color
 * @param g     green component of color
 * @param b     blue component of color
 *
 * @return   the color as a single value of type color_t
 */
color_t gl_color(uint8_t r, uint8_t g, uint8_t b);

/*
 * `gl_clear`
 *
 * Clear all the pixels in the framebuffer to the given color.
 *
 * @param c     color to set all pixels
 */
void gl_clear(color_t c);

/*
 * `gl_swap_buffer`
 *
 * If in double-buffered mode, all gl drawing takes place in the
 * off-screen buffer and updated drawing is not brought on-screen until
 * a call is made to `gl_swap_buffer` to exchange the on-screen
 * and off-screen buffers.
 *
 * If not in double-buffer mode, all drawing takes place on-screen and
 * the `gl_swap_buffer` function has no effect.
 */
void gl_swap_buffer(void);

/*
 * `gl_draw_pixel`
 *
 * Draw a single pixel at location x,y in color c.
 * If the location is outside the bounds of framebuffer, it is not drawn.
 *
 * @param x     x location of pixel
 * @param y     y location of pixel
 * @param c     color of pixel
 */
void gl_draw_pixel(int x, int y, color_t c);

/*
 * `gl_read_pixel`
 *
 * Return the color of the pixel at location x,y. Returns 0 if the
 * location is outside the bounds of the framebuffer.
 *
 * @param x     x location of pixel
 * @param y     y location of pixel
 *
 * @return   the color at that location
 */
color_t gl_read_pixel(int x, int y);

/*
 * `gl_draw_char`
 *
 * Draw a single character at location x,y in color c.
 * Only those pixels of the character that lie within the bounds
 * of the framebuffer are drawn. Any pixel that lies outside is
 * clipped (i.e. not drawn). Only the "on" pixels of the character
 * are drawn, all "off" pixels are left as-is.
 *
 * @param x     x location of upper left corner of character glyph
 * @param y     y location of upper left corner of character glyph
 * @param ch    character to be drawn, e.g. 'a'. If this character has no glyph
 *              in the current font, nothing is drawn (refer to font_get_glyph())
 * @param c     color of the character
 */
void gl_draw_char(int x, int y, char ch, color_t c);

/*
 * `gl_draw_string`
 *
 * Draw a string at location x,y in color c. The characters are drawn
 * left to right in a single line. Only the pixels of the characters
 * that lie within the bounds of the framebuffer are drawn. Any pixel
 * that lies outside is clipped (i.e. not drawn). Only the "on" pixels of
 * the characters are drawn, all "off" pixels are left as-is.
 *
 * @param x     x location of upper left corner of the first char of string
 * @param y     y location of upper left corner of the first char of string
 * @param str   null-terminated string to be drawn
 * @param c     color of the string
 */
void gl_draw_string(int x, int y, const char* str, color_t c);

/*
 * `gl_get_char_height`
 *
 * Get the height in pixels of a single character glyph.
 *
 * @return  the height of character glyph in pixels
 */
int gl_get_char_height(void);

/*
 * `gl_get_char_width`
 *
 * Get the width in pixels of a single character glyph.
 *
 * @return  the width of character glyph in pixels
 */
int gl_get_char_width(void);

/*
 * `gl_draw_rect`
 *
 * Draw a filled rectangle at location x,y with size w,h filled with color c.
 * All pixels in the rectangle that lie within the bounds of the
 * framebuffer are drawn. Any pixel that lies outside is clipped (i.e. not drawn).
 *
 * @param x     x location of upper left corner of the rectangle
 * @param y     y location of upper left corner of the rectangle
 * @param w     width of rectangle
 * @param h     height of rectangle
 * @param c     color of rectangle
 */
void gl_draw_rect(int x, int y, int w, int h, color_t c);

#endif
