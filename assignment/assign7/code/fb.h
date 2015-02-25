#ifndef FB_H_INCLUDED
#define FB_H_INCLUDED

int fb_init();
char* fb_writeable_buffer();
unsigned int fb_byte_depth();
unsigned int fb_height();
unsigned int fb_width();

#endif
