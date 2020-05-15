#include "assert.h"
#include "gpio.h"
#include "mailbox.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

typedef struct {
    unsigned int width;       // width of the display (height/width expressed in pixels)
    unsigned int height;      // height of the display
    unsigned int virtual_width;  // width of the virtual framebuffer
    unsigned int virtual_height; // height of the virtual framebuffer
    unsigned int pitch;       // number of _bytes_ per row
    unsigned int bit_depth;   // number of _bits_ per pixel
    unsigned int x_offset;    // x of the upper left corner of the virtual framebuffer
    unsigned int y_offset;    // y of the upper left corner of the virtual framebuffer
    void *framebuffer;        // pointer to the start of the framebuffer
    unsigned int total_bytes; // total number of _bytes_ in the framebuffer
} fb_config_t;

static volatile fb_config_t fb __attribute__((aligned(16)));

static const int WIDTH = 640;
static const int HEIGHT = 480;
static const int DEPTH = 32;

void fb_init(void) {
    fb.width = WIDTH;
    fb.height = HEIGHT;
    fb.virtual_width = WIDTH;
    fb.virtual_height = HEIGHT;
    fb.bit_depth = DEPTH;
    fb.x_offset = 0;
    fb.y_offset = 0;

    // these must be 0
    fb.framebuffer = 0;
    fb.total_bytes = 0;
    fb.pitch = 0;

    mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb);
    assert(mailbox_read(MAILBOX_FRAMEBUFFER) == 0);
}

void clear_char_by_char(void)
{
    unsigned char *im = fb.framebuffer;
    // write each byte in the framebuffer
    for (unsigned int i = 0; i < fb.total_bytes; i++) {
        *im++ = 0xff; // white
    }
}

void clear_int_by_int(void)
{
    unsigned int *im = fb.framebuffer;
    // compute total number of unsigned ints in the framebuffer
    int nwords = fb.total_bytes/sizeof(unsigned int); 
    for (int i = 0; i < nwords; i++) { 
        *im++ = 0xff0000ff; // blue
    }
}

void clear_2d(void)
{
    unsigned int row_len = fb.pitch/sizeof(unsigned int); // pitch is in bytes, translate to count of words
    unsigned int (*im)[row_len] = fb.framebuffer;
    for (int y = 0; y < fb.height; y++) {
        for (int x = 0; x < row_len; x++) {
            im[y][x] = 0xffff0000; // red
        }
    }
}

void main(void) {
    gpio_init();
    timer_init();
    uart_init();

    fb_init();

    printf ("width (pixels): %d\n", (int)fb.width);
    printf ("height (pixels): %d\n", (int)fb.height);
    printf ("depth (bits): %d\n", (int)fb.bit_depth);
    printf ("total bytes: %d\n", (int)fb.total_bytes);
    printf ("pitch (bytes): %d\n", (int)fb.pitch);

    clear_char_by_char();
    timer_delay(1);
    clear_int_by_int();
    timer_delay(1);
    clear_2d();
    timer_delay(1);

    uart_putchar(EOT);
}
