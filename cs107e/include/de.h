#ifndef DE_H
#define DE_H

/*
 * Functions for interacting with Display Engine peripheral.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */
void de_init(int width, int height);
void de_set_active_framebuffer(void *addr);

#endif
