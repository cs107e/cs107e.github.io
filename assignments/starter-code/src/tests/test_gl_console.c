#include "assert.h"
#include "console.h"
#include "fb.h"
#include "gl.h"
#include "printf.h"
#include "strings.h"
#include "timer.h"
#include "uart.h"


#define _WIDTH 640
#define _HEIGHT 512

#define _NROWS 10
#define _NCOLS 20

void test_fb(void)
{
    fb_init(700, 700, 4, FB_SINGLEBUFFER);
    printf("Frame buffer pitch is %d\n", fb_get_pitch());

    unsigned char *cptr = fb_get_draw_buffer();
    int nbytes = fb_get_pitch()*fb_get_height();
    memset(cptr, 0x99, nbytes); // fill entire framebuffer with light gray pixels
    timer_delay(3);
}

void test_gl(void)
{
    // Double buffer mode, make sure you test single buffer too!
    gl_init(_WIDTH, _HEIGHT, GL_DOUBLEBUFFER);

    // Background is purple
    gl_clear(gl_color(0x55, 0, 0x55));

    // Draw green pixel at an arbitrary spot
    gl_draw_pixel(_WIDTH/3, _HEIGHT/3, GL_GREEN);
    assert(gl_read_pixel(_WIDTH/3, _HEIGHT/3) == GL_GREEN);

    // Blue rectangle in center of screen
    gl_draw_rect(_WIDTH/2 - 50, _HEIGHT/2 - 50, 100, 100, GL_BLUE);

    // Single amber character
    gl_draw_char(60, 10, 'A', GL_AMBER);

    // Show buffer with drawn contents
    gl_swap_buffer();
    timer_delay(3);
}

void test_console(void)
{
    console_init(_NROWS, _NCOLS);

    // 1: "HELLO WORLD"
    console_printf("HELLO WORLD\r");
    timer_delay(3);

    // 1: "HAPPY WORLD"
    // 2: "CODING"
    console_printf("HAPPY\nCODING\n");
    timer_delay(3);

    // Clear
    console_printf("\f");

    // 1: "Goodbye"
    console_printf("Goodbye!\n");
}

/* TODO: Add tests to test your graphics library and console.
   For the graphics library, test both single & double
   buffering and confirm all drawing is clipeed to bounds
   of framebuffer
   For the console, make sure to test wrap-around and scrolling.
   Be sure to test each module separately as well as in combination
   with others.
*/

void main(void)
{
    uart_init();
    timer_init();
    printf("Executing main() in test_gl_console.c\n");

    test_fb();
    test_gl();
    test_console();

    printf("Completed main() in test_gl_console.c\n");
    uart_putchar(EOT);
}
