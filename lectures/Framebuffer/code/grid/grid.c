#include "fb.h"
#include "gl.h"

const unsigned WIDTH = 1280;
const unsigned HEIGHT = 1024;
const unsigned DEPTH = 4;

// 2D
void draw_pixel(int x, int y, color_t c)
{
    unsigned int width = fb_get_pitch()/4;
    unsigned int (*im)[width] = (unsigned int (*)[WIDTH])fb_get_draw_buffer();

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
      draw_hline(y, GL_WHITE);
  for( int x = 0; x < WIDTH; x += 16 )
      draw_vline(x, GL_WHITE);

}

