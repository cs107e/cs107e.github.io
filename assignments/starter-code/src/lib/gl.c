#include "gl.h"

void gl_init(unsigned int width, unsigned int height, gl_mode_t mode)
{
    fb_init(width, height, 4, mode);    // use 32-bit depth always for graphics library
}

void gl_swap_buffer(void)
{
    // TODO: implement this function
}

unsigned int gl_get_width(void)
{
    // TODO: implement this function
    return 0;
}

unsigned int gl_get_height(void)
{
    // TODO: implement this function
    return 0;
}

color_t gl_color(unsigned char r, unsigned char g, unsigned char b)
{
    // TODO: implement this function
    return 0;
}

void gl_clear(color_t c)
{
    // TODO: implement this function
}

void gl_draw_pixel(int x, int y, color_t c)
{
    // TODO: implement this function
}

color_t gl_read_pixel(int x, int y)
{
    // TODO: implement this function
    return 0;
}

void gl_draw_rect(int x, int y, int w, int h, color_t c)
{
    // TODO: implement this function
}

void gl_draw_char(int x, int y, int ch, color_t c)
{
    // TODO: implement this function
}

void gl_draw_string(int x, int y, const char* str, color_t c)
{
    // TODO: implement this function
}

unsigned int gl_get_char_height(void)
{
    // TODO: implement this function
    return 0;
}

unsigned int gl_get_char_width(void)
{
    // TODO: implement this function
    return 0;
}
