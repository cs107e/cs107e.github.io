#include "fb.h"

typedef unsigned int color_t;

static const color_t WHITE = 0xffffffff;
static const int WIDTH = 640;
static const int HEIGHT = 512;
static const int DEPTH = 4; // in bytes

void draw_pixel(int x, int y, color_t c) {
	unsigned int row_width = fb_get_pitch()/DEPTH;
	unsigned int (*screen)[row_width] = fb_get_draw_buffer();
	screen[y][x] = c;
}

void draw_hline(int y, color_t c) {
	int width = fb_get_width();
    for(int x = 0; x < width; x++ ) {
        draw_pixel(x, y, c);
	}
}

void draw_vline(int x, color_t c)
{
	int height = fb_get_height();
	for (int y = 0; y < height; y++) {
		draw_pixel(x, y, c);
	}
}


void main(void) 
{
    fb_init(WIDTH, HEIGHT, DEPTH, FB_SINGLEBUFFER);

	for (int y = 0; y < HEIGHT; y += 32) {
		draw_hline(y, WHITE);
	}

	for (int x = 0; x < WIDTH; x += 32) {
		draw_vline(x, WHITE);
	}
}

