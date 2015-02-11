/* Implement frame buffer from fb.h here. */

#include "fb.h"

typedef struct {
} fb_config_t;

fb_config_t config __attribute__ ((aligned(16)));

int fb_init() {return 0;}

/* Return a pointer to the off-screen buffer. */

char* fb_writeable_buffer() {return 0;}
unsigned int fb_byte_depth() {return 0;}
unsigned int fb_height() {return 0;}
unsigned int fb_width() {return 0;}

/* Clear the off-scren buffer.*/
void fb_clear() {}

/* Swap the on-screen and off-screen buffers. */
void fb_draw() {}

/* Return a pointer to the on-screen buffer. */
const char* fb_display() {return 0;}

