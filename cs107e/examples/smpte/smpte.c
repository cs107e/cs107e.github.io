/*
 * This application displays the NSTC test pattern.
 * https://en.wikipedia.org/wiki/SMPTE_color_bar
 * http://avisynth.nl/index.php/ColorBars_theory
 *
 */
#include "gl.h"

#define WIDTH  840
#define HEIGHT 600
#define DEPTH 4 

static color_t intensity(color_t c, float percentage)
{
    unsigned char blue = (c & 0xff) * percentage;
    unsigned char green = ((c >> 8) & 0xff) * percentage;
    unsigned char red = ((c >> 16) & 0xff) * percentage;
    return gl_color(red, green, blue);
}

void draw_smpte_bars(void)
{
    #define N 7
    color_t bars[N] = {
        intensity(GL_WHITE, .75),
        intensity(GL_YELLOW, .75),
        intensity(GL_CYAN, .75),
        intensity(GL_GREEN, .75),
        intensity(GL_MAGENTA, .75),
        intensity(GL_RED, .75),
        intensity(GL_BLUE, .75),
    };
    color_t castellations[N] = {
        intensity(GL_BLUE, .75),
        intensity(GL_WHITE, .075),
        intensity(GL_MAGENTA, .75),
        intensity(GL_WHITE, .075),
        intensity(GL_CYAN, .75),
        intensity(GL_WHITE, .075),
        intensity(GL_WHITE, .75),
   };
   color_t luminance[N+1] = { // indexes 3-5 pluge (Picture Line-Up Generation Equipment)
        gl_color(0, 58, 98), //-I 303 degr
        GL_WHITE,
        gl_color(59, 15, 126), // Q 33 degr
        intensity(GL_WHITE, .075), //  0
        intensity(GL_WHITE, .035), // -4
        intensity(GL_WHITE, .075), //  0
        intensity(GL_WHITE, .115), // +4
        intensity(GL_WHITE, .075), //  0
   };

   unsigned bar_width = WIDTH/N;
   unsigned bar_height = 2*HEIGHT/3;
   unsigned y = 0;

   for (int i = 0; i < N; i++) {
        gl_draw_rect(i*bar_width, y, bar_width, bar_height, bars[i]);
   }

   y += bar_height;
   unsigned cast_height = HEIGHT/12;
   for (int i = 0; i < N; i++) {
        gl_draw_rect(i*bar_width, y, bar_width, cast_height, castellations[i]);
   }

   y += cast_height;
   int x = 0;
   for (int i = 0; i < 8; i++) {
        unsigned int w = (i < 4) ? (bar_width*5)/4 : ((i <= 6) ? bar_width/3 : bar_width);
        gl_draw_rect(x, y, w, HEIGHT-y, luminance[i]);
        x += w;
   }
}


void main(void) 
{ 
    gl_init(WIDTH, HEIGHT, GL_SINGLEBUFFER);
    gl_clear(GL_AMBER);
    draw_smpte_bars();
}
