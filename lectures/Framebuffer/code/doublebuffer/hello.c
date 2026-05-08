#include "gl.h"
#include "printf.h"
#include "uart.h"

void main(void) {
    uart_init();
    gl_init(300, 200, GL_SINGLEBUFFER); // change to GL_DOUBLEBUFFER instead

    printf("Enter any key to exit: ");
    while (!uart_haschar()) {
        gl_clear(GL_BLACK);
        gl_draw_string(50, 100, "hello, world | |", GL_BLUE);
        gl_swap_buffer();

        gl_clear(GL_BLACK);
        gl_draw_string(50, 100, "hello, world  + +", GL_RED);
        gl_swap_buffer();
    }
    printf("%c\nExiting %s\n", uart_getchar(), __FILE__);
}
