#include "fb.h"
#include "gl.h"
#include "printf.h"
#include "uart.h"

static void estonian_flag(int x, int y, int w, int h) {
    const int n = 3;
    color_t colors[] = {GL_BLUE, GL_BLACK, GL_WHITE};
    h /= n;
    for (int i = 0; i < n; i++) {
        gl_draw_rect(x, y, w, h, colors[i]);
        y += h;
    }
}

void main(void) {
    uart_init();
    gl_init(400, 300, GL_SINGLEBUFFER);

    gl_clear(0xff666666);
    for (int line = 0; line < 100; line++) {
        gl_draw_string(line *gl_get_char_width(), line*gl_get_char_height(), "Vaba eesti", GL_GREEN);
    }
    estonian_flag(100, 100, fb_get_width()-200, fb_get_height()-200);

    printf("Hit any key to quit: ");
    uart_getchar();
    printf("\nCompleted %s\n", __FILE__);
}