#ifndef FONT_H
#define FONT_H

/*
 * This module provides a font.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 */

#include <stdbool.h>
#include <stddef.h>

/*
 * A font contains a glyph for each ASCII character to be drawn.
 * A glyph is a bitmap image for drawing the character.
 *
 * The font is fixed width; every glyph has the same width and
 * same height. A glyph is an image with font_get_glyph_height() rows
 * and font_get_glyph_width() columns.
 *
 * Each pixel of the glyph image is represented as one byte.
 * The byte for an "on" pixel is 0xFF, and the byte for
 * "off" pixel is 0x00.
 */

/*
 * `font_get_glpyh_height`
 *
 * Get the height in pixels of each glyph in this font.
 *
 * @return    the height in pixels
 */
int font_get_glyph_height(void);

/*
 * `font_get_glpyh_width`
 *
 * Get the width in pixels of each glyph in this font.
 *
 * @return    the width in pixels
 */
int font_get_glyph_width(void);

/*
 * `font_get_glpyh_size`
 *
 * Get the total number of bytes needed to store the glyph image
 * for one character. This is equal to the product of
 * (glyph height * glyph width).
 *
 * @return    the size in bytes of a glyph image
 */
int font_get_glyph_size(void);

/*
 * `font_get_glpyh`
 *
 * Fill in the glyph image for character `ch` into `buf`.
 * `buf` is an array of bytes of length font_get_glyph_size().
 * Each 'on' pixel has value 0xff, 'off' pixel has value 0x0.
 *
 * @param ch    the requested character
 * @param buf   the buffer in which to place the glyph image
 * @param buflen the length of `buf`.
 *              `buflen` should be equal to value returned by font_get_glyph_size()
 *
 * @return      returns true when successfully filled `buf`, false otherwise.
 *              Failure is when `ch` is not available in this font or `buflen`
 *              does not equal the value returned by font_get_glyph_size()
 */
bool font_get_glyph(char ch, unsigned char buf[], size_t buflen);

#endif
