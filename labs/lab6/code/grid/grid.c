#include "fb.h"

typedef unsigned int color_t;

static const color_t WHITE = 0xffffffff;
static const int WIDTH = 640;
static const int HEIGHT = 512;
static const int DEPTH = 4;

void draw_pixel(int x, int y, color_t c)
{
    unsigned int per_row = fb_get_pitch()/DEPTH; // length of each row in pixels (include any padding)
    unsigned int (*im)[per_row] = fb_get_draw_buffer();
    im[y][x] = c;
}

void draw_hline(int y, color_t c)
{
    for( int x = 0; x < fb_get_width(); x++ )
        draw_pixel(x, y, c);
}

void draw_vline(int x, color_t c)
{
    for( int y = 0; y < fb_get_height(); y++ )
        draw_pixel(x, y, c);
}


void main(void) 
{
    fb_init(WIDTH, HEIGHT, DEPTH, FB_SINGLEBUFFER);

    for( int y = 0; y < HEIGHT; y += 16 )
        draw_hline(y, WHITE);

    for( int x = 0; x < WIDTH; x += 16 )
        draw_vline(x, WHITE);
}

