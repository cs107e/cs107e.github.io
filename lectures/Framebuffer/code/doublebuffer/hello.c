#include "gl.h"
#include "printf.h"
#include "uart.h"

void main(void) {
    uart_init();
    gl_init(800, 600, GL_DOUBLEBUFFER); // change to GL_DOUBLEBUFFER instead

    printf("Enter any key to exit: ");
    while (!uart_haschar()) {
        gl_clear(GL_BLACK);
        gl_draw_string(20, 20, "hello, world 0", GL_WHITE);
        gl_swap_buffer();

        gl_clear(GL_BLACK);
        gl_draw_string(20, 20, "hello, world 1", GL_WHITE);
        gl_swap_buffer();
    }
    printf("%c\nExiting %s\n", uart_getchar(), __FILE__);
}
