#ifndef FB_H_INCLUDED
#define FB_H_INCLUDED

int fb_init();
volatile char* fb_writeable_buffer();
unsigned fb_byte_depth();
unsigned fb_height();
unsigned fb_width();

#endif
