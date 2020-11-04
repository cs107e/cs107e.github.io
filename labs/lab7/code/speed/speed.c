/* File speed.c
 * -----------
 * Simple main program with timing code to measure
 * performance of different implementations of redraw.
 */

#include "gl.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

// -------------------------------------------------------------------
// Time Trial Helpers
// -------------------------------------------------------------------


unsigned _time_trial(void (*fn)(color_t c))
{
    static int nrefresh = 0;
    color_t cycle[3] = {GL_RED, GL_WHITE, GL_BLUE};
    color_t c = cycle[nrefresh++ % 3];

    gl_clear(GL_BLACK);
    unsigned start = timer_get_ticks();
    fn(c);
    unsigned elapsed = timer_get_ticks() - start;
    return elapsed;
}

#define TIME_TRIAL(fn) do {           \
    printf("Running " #fn "... ");    \
    printf("took %d ticks\n", _time_trial(fn)); \
} while (0)


// -------------------------------------------------------------------
// Baseline redraw0, correct implementation but pokey
// -------------------------------------------------------------------

void redraw0(color_t c)
{
    for (int y = 0; y < gl_get_height(); y++) {
        for (int x = 0; x < gl_get_width(); x++) {
            gl_draw_pixel(x, y, c);
        }
    }
}

// -------------------------------------------------------------------
// Wicked fast redraw functions of your own go here:
// -------------------------------------------------------------------


void main(void) 
{   
    timer_init();
    uart_init();
    gl_init(640, 480, GL_SINGLEBUFFER);

    printf("\nStarting time trials now.\n");

    TIME_TRIAL(redraw0);

    // TODO: Add more TIME_TRIAL calls here for your improved versions
    // TIME_TRIAL(redraw1)
    // TIME_TRIAL(redraw2)

    printf("\nAll done with time trials.\n");
    uart_putchar(EOT);
}
