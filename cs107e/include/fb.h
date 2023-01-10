#ifndef FB_H
#define FB_H

/*
 * Low-level frameuffer routines for controlling a bare metal
 * Raspberry Pi's graphics. Presents a useful abstraction of hardware
 * that a graphics library can then use to provide useful drawing
 * primitives.
 *
 * Students implement this module for assignment 6.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: Mar 23 2016
 */

typedef enum { FB_SINGLEBUFFER = 0, FB_DOUBLEBUFFER = 1 } fb_mode_t;

/*
 * `fb_init` : Required initialized for framebuffer
 *
 * Initialize the framebuffer.
 *
 * @param width  the requested width in pixels of the framebuffer
 * @param height the requested height in pixels of the framebuffer
 * @param depth  the requested depth in bytes of each pixel
 * @param mode   whether the framebuffer should be
 *                      single buffered (FB_SINGLEBUFFER)
 *                      or double buffered (FB_DOUBLEBUFFER)
 */
void fb_init(unsigned int width, unsigned int height, unsigned int depth_in_bytes, fb_mode_t mode);

/*
 * `fb_get_width`
 *
 * Get the current width in pixels of the framebuffer.
 *
 * @return    the width in pixels
 */
unsigned int fb_get_width(void);

/*
 * `fb_get_height`
 *
 * Get the current height in pixels of the framebuffer.
 *
 * @return    the height in pixels
 */
unsigned int fb_get_height(void);

/*
 * `fb_get_depth`
 *
 * Get the current depth in bytes of a single pixel.
 *
 * @return    the depth in bytes
 */
unsigned int fb_get_depth(void);

/*
 * `fb_get_pitch`
 *
 * Get the current pitch in bytes of a single row of pixels in the framebuffer.
 * The pitch is nominally the width (number of pixels per row) multiplied by
 * the depth in bytes per pixel. However, the pitch may be greater than that if the
 * GPU elects to add padding to the end of the row.
 *
 * @return    the pitch in bytes
 */
unsigned int fb_get_pitch(void);

/*
 * `fb_get_draw_buffer`
 *
 * Get the start address of the framebuffer memory into which the
 * client can draw pixels.  The address returned is the start of an
 * array of size pitch*height bytes.
 *
 * If in single buffering mode, there is only one buffer in use, so the
 * returned address does not change. That buffer is on-screen at all
 * times. The client draws to that one buffer, and all updates are
 * immediately displayed.
 *
 * In double buffering mode, there are two buffers: the one
 * currently on-screen and a second buffer that is off-screen. The
 * address returned by `fb_get_draw_buffer` corresponds to the buffer
 * that is currently off-screen. The off-screen buffer is sometimes
 * called the draw buffer. The client will do all drawing to the off-screen
 * buffer and when ready, calls `fb_swap_buffer` to exchange the
 * on-screen and off-screen buffers. The swap brings the updated
 * drawing on-screen in one smooth update.
 *
 * Note the address is returned as `void*`. Client should store into
 * a properly typed pointer so as to access the pixel data according
 * to their desired scheme (1-d, 2-d, etc.)
 *
 * @return    the address of the current draw (off-screen) buffer
 */
void* fb_get_draw_buffer(void);

/*
 * `fb_swap_buffer`
 *
 * Swap the on-screen and off-screen buffers. The draw buffer (off-screen)
 * is moved on-screen (contents now displayed) and on-screen buffer
 * is moved off-screen (becomes the draw buffer, contents off-screen).
 *
 * If not in double buffering mode, there is only one buffer and this
 * function has no effect.
 */
void fb_swap_buffer(void);

#endif
