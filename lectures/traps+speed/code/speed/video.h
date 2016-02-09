#ifndef VIDEO_H_INCLUDED
#define VIDEO_H_INCLUDED

int fb_init();
volatile char* fb_writeable_buffer();
unsigned int fb_byte_depth();
unsigned int fb_height();
unsigned int fb_width();

#endif
