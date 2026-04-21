#ifndef DE_H
#define DE_H

/*
 * Functions for interacting with Display Engine peripheral.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */


/*
 * `de_init` : Required initializion for DisplayEngine
 *
 * The DE is configured to display a framebuffer of the specified
 * width and height on a screen of the specified dimensions.
 * DE should be initialized only after HDMI hardware has been configured
 * via hdmi_init(). If the requested fb dimensions do not fit on the
 * screen, an error is raised.
 *
 * @param fb_width      width of framebuffer in pixels
 * @param fb_height     height of framebuffer in pixels
 * @param screen_width  width of hdmi screen in pixels
 * @param screen_height height of hdmi screen in pixels
 */
void de_init(int fb_width, int fb_height, int screen_width, int screen_height);

/*
 * `de_set_active_framebuffer` :
 *
 * Change screen to show contents of framebuffer memory at `addr`.
 * Framebuffer is assumed to be valid memory of size/shape matching how
 * DE was configured during de_init().
 *
 * @param addr      base address of framebuffer to display
 */
void de_set_active_framebuffer(void *addr);

#endif
