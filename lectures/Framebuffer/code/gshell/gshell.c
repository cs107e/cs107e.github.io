/* 
 * File: gshell.c
 * -------------------
 * 
 * Starts the shell and the frame buffer in single buffered mode.
 * Also draws something on the frame buffer so you know it is working.
 * Prints out the framebuffer address over the UART so you can then
 * poke it from the shell, to show it's just memory.
 * 
 * 
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: 5/15/2020
 */
#include "keyboard.h"
#include "printf.h"
#include "shell.h"
#include "uart.h"
#include "fb.h"
#include "gl.h"


void main(void)
{
    uart_init();
    gl_init(640, 480, GL_SINGLEBUFFER);
    
    gl_draw_rect(10, 470, 630, 5, GL_RED);
    gl_draw_line(10, 10, 630, 480, GL_GREEN);

    printf("\nStarting %s\n", __FILE__);
    printf("Frame buffer is at 0x%x\n", (unsigned int)fb_get_draw_buffer());
    
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
    shell_init(printf);

    shell_run();

    printf("\nCompleted %s\n", __FILE__);
    uart_putchar(EOT);
}
