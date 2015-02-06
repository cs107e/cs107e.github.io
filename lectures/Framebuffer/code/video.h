#ifndef VIDEO_H_INCLUDED
#define VIDEO_H_INCLUDED

int video_init();
char* video_writeable_buffer();
unsigned int video_byte_depth();
unsigned int video_height();
unsigned int video_width();

#endif
