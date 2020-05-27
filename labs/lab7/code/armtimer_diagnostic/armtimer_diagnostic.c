/*
 * Application to test interrupts assembly.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: May 13, 2020
 */
 
#include "gl.h"
#include "interrupts.h"
#include "printf.h"
#include "uart.h"
#include "armtimer.h"
#include "backtrace.h"

// Need a static global because it's shared between the main loop
// and interrupt handler.
static volatile unsigned int ticks = 0;

static bool alarm(unsigned int pc) {
  // This is the same code you used in backtrace to fetch
  // the frame pointer, except that it fetches the stack
  // pointer. This is useful here in case you are not saving/
  // restoring your stack properly. If so, you will see the 
  // stack pointer slowly grow or shrink as the interrupt
  // executes again and again.
  char *sp;
  __asm__("mov %0, sp" : "=r"(sp));

  // You'd normally never do a printf in an interrupt handler:
  // they are supposed to be fast and your serial port if very slow.
  // But it can be fine to do so as a diagnostic (like here) or when trying
  // to figure out a bug.
  if (armtimer_check_and_clear_interrupt()) {
    printf("Alarm interrupt: pc=0x%x sp=0x%x.\n", pc, (unsigned int)sp);
    ticks++;
    print_backtrace();
    printf("\n");
    return true;
  } else {
    return false;
  }
}

void redraw(void)
{
    static unsigned int nrefresh = 0;
    color_t cycle[3] = {GL_RED, GL_WHITE, GL_BLUE};
    color_t c = cycle[ticks % 3];
    printf("Redraw iteration #%d, ticks=%d\n", nrefresh++, ticks);
    unsigned int max_x = gl_get_width();
    unsigned int max_y = gl_get_height();
    for (int y = 0; y < max_y; y++) {
        for (int x = 0; x < max_x; x++) {
            gl_draw_pixel(x, y, c);
        }
    }
}

void main(void) 
{
    uart_init();
    interrupts_init();
    gl_init(640, 480, GL_SINGLEBUFFER);

    
    armtimer_init(250000); // 250,000 usec = 4Hz
    armtimer_enable();
    armtimer_enable_interrupts();
    handler_fn_t old = interrupts_register_handler(INTERRUPTS_BASIC_ARM_TIMER_IRQ, alarm);
    interrupts_enable_source(INTERRUPTS_BASIC_ARM_TIMER_IRQ);

    printf("Interrupts configured: old armtimer handler was 0x%x\n", (unsigned int)old);

    interrupts_global_enable();
    while (1) {
        redraw();
    }
}
