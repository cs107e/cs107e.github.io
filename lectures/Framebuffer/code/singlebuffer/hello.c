#include "gl.h"

void main(void) {
    gl_init(1280, 1024, GL_SINGLEBUFFER);

    while (1) {
        gl_clear(GL_BLACK);
        gl_draw_string(20, 20, "hello, world", GL_WHITE);
    }
}
