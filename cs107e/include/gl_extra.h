#ifndef GL_EXTRA_H
#define GL_EXTRA_H

/*
 * Optional extended features for gl supported by the
 * reference module.
 *
 * These functions are NOT part of the core requirements
 * for Assignment 6.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "gl.h"

/*
 * `gl_draw_line`: draw anti-aliased line from point to point
 *
 * Draw a line segment from location x1,y1 to location x2,y2 of color c.
 * All pixels along the line that lie within the bounds of the framebuffer
 * are drawn. Any pixel that lies outside is clipped (i.e. not drawn).
 *
 * @param x1  x location of vertex 1
 * @param y1  y location of vertex 1
 * @param x2  x location of vertex 2
 * @param y2  y location of vertex 2
 * @param c   color of line
 */
void gl_draw_line(int x1, int y1, int x2, int y2, color_t c);

/*
 * `gl_draw_triangle`: draw filled triangle
 *
 * Draw a filled triangle connecting the three vertices filled with color c.
 * All pixels within the triangle that lie within the bounds of the
 * framebuffer are drawn. Any pixel that lies outside is clipped (i.e. not drawn).
 *
 * @param x1  x location of vertex 1
 * @param y1  y location of vertex 1
 * @param x2  x location of vertex 2
 * @param y2  y location of vertex 2
 * @param x3  x location of vertex 3
 * @param y3  y location of vertex 3
 * @param c   color of triangle
 */
void gl_draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, color_t c);

/*
 * `gl_draw_oval`: draw filled oval
 *
 * Draw a filled oval centered at x,y with size w,h filled with color c.
 * All pixels within the oval that lie within the bounds of the
 * framebuffer are drawn. Any pixel that lies outside is clipped (i.e. not drawn).
 *
 * @param x   x location of oval's center
 * @param y   y location of oval's center
 * @param w   width of oval
 * @param h   height of oval
 * @param c   color of oval
 */
void gl_draw_oval(int x, int y, int w, int h, color_t c);

struct image {
    int width, height;
    uint32_t *pixel_data; // BGRA pixels arranged row-major order
};

/*
 * `gl_draw_image`: draw image
 *
 * Draw given image at location upper left x,y.
 * All pixels within the image that lie within the bounds of the
 * framebuffer are drawn. Any pixel that lies outside is clipped (i.e. not drawn).
 *
 * @param x     x location of image's upper left corner
 * @param y     y location of image's upper left corner
 * @param img   pointer to struct containing image info
 */
void gl_draw_image(int x, int y, const struct image *img);

/*
 * `gl_reset_glyph_cache`: clears the internal glyph cache to reset the font
 *
 * For efficiency reasons, gl keeps a cache of glyphs harvested from the current font.
 * After changing font, we need gl to clear the cache and allow it to repopulate
 * with glyphs from new font.
 */
void gl_reset_glyph_cache(void);

#endif
