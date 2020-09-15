#ifndef FB_REF
#define FB_REF
#include "fb.h"
void ref_fb_init(unsigned int width, unsigned int height, unsigned int depth_in_bytes, fb_mode_t mode);
unsigned int ref_fb_get_width(void);
unsigned int ref_fb_get_height(void);
unsigned int ref_fb_get_depth(void);
unsigned int ref_fb_get_pitch(void);
void* ref_fb_get_draw_buffer(void);
void ref_fb_swap_buffer(void);
#endif
