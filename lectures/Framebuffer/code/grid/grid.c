#include "fb.h"
#include "gl.h"
#include "printf.h"
#include "uart.h"

void draw_pixel(int x, int y, color_t c) {
    unsigned int per_row = fb_get_width();
    unsigned int (*im)[per_row] = fb_get_draw_buffer();
    im[y][x] = c;
}

void draw_hline(int y, color_t c) {
    for (int x = 0; x < fb_get_width(); x++) {
        draw_pixel(x, y, c);
    }
}

void draw_vline(int x, color_t c) {
    for (int y = 0; y < fb_get_height(); y++) {
        draw_pixel(x, y, c);
    }
}

void main(void)  {
    uart_init();
    fb_init(800, 600, FB_SINGLEBUFFER);

    color_t blue = 0x0000ff;
    for (int y = 0; y < fb_get_height(); y += 25) {
        draw_hline(y, blue);
    }

    color_t red = 0xff0000;
    for (int x = 0; x < fb_get_width(); x += 25) {
        draw_vline(x, red);
    }
    printf("Hit any key to quit: ");
    uart_getchar();
    printf("\nCompleted %s\n", __FILE__);
}
