#include <stdio.h>
#include "fb.h"


const color WHITE = 0xffffffff;
const unsigned WIDTH = 1280;
const unsigned HEIGHT = 1024;
const unsigned DEPTH = 4;

void draw_pixel(int x, int y, color c)
{
    unsigned (*im)[WIDTH] = (unsigned (*)[WIDTH])fb_get_draw_buffer();
    im[y][x] = c;
}

/*
void draw_pixel(int x, int y, color c)
{
    unsigned *im = (unsigned *)fb_get_draw_buffer();
    *(im + y * WIDTH + x) = c; 
}
*/

void draw_hline(int y, color c)
{
    for( int x = 0; x < fb_get_width(); x++ )
        draw_pixel(x, y, c);
}

void draw_vline(int x, color c)
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

