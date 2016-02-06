#ifndef FB_H
#define FB_H

#define FB_SINGLEBUFFER 0
#define FB_DOUBLEBUFFER 1

typedef unsigned color;

void fb_init(unsigned width, unsigned height, unsigned depth,
             unsigned doublebuffer);

unsigned fb_get_depth(void);
unsigned fb_get_height(void);
unsigned fb_get_width(void);

unsigned char* fb_get_draw_buffer(void);
unsigned char* fb_get_front_buffer(void);
unsigned char* fb_get_back_buffer(void);
void fb_swap_buffer(void);

void fb_clear(color c);

#endif
