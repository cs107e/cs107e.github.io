#include "assert.h"
#include "gpio.h"
#include "mailbox.h"
#include "printf.h"
#include "uart.h"

typedef struct {
    unsigned int width;       // physical width of the display (pixels)
    unsigned int height;      // physical height of the display (pixels)
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

//static const int WIDTH = 625; // appears to align with a multiple of 64
static const int WIDTH = 640;
static const int HEIGHT = 480;
static const int DEPTH = 32;   // depth in ** BITS ** = 4 bytes

void fb_init(void) {
    // write these values to the GPU
    fb.width = 1600;
    fb.height = 1200;
    fb.virtual_width = WIDTH;
    fb.virtual_height = HEIGHT;
    fb.bit_depth = DEPTH;
    fb.x_offset = 0;
    fb.y_offset = 0;

    // read these values from the GPU - must be 0 initially
    fb.framebuffer = 0;
    fb.total_bytes = 0;
    fb.pitch = 0;

    // send the request to the GPU using "mail"
    bool mailbox_success = mailbox_request(MAILBOX_FRAMEBUFFER, (unsigned)&fb);
    assert(mailbox_success);
}

void main(void) {
    gpio_init();
    uart_init();

    printf("requested physical size = (%d, %d)\n", 1600, 1200);
    printf("requested virtual size = (%d, %d)\n", WIDTH, HEIGHT);
    printf("requested depth = %d bits (%d bytes)\n", DEPTH, DEPTH / 8);

    fb_init();

    printf("physical size = (%d, %d)\n", fb.width, fb.height);
    printf("virtual size = (%d, %d)\n", fb.virtual_width, fb.virtual_height);
    printf("depth (bits) = %d\n", fb.bit_depth);
    printf("framebuffer address = %p\n", fb.framebuffer);
    printf("framebuffer total bytes = %d (height*width*depth=%d)\n",
        fb.total_bytes, fb.virtual_height*fb.virtual_width*fb.bit_depth/8);
    printf("pitch (number of bytes in a scanline)= %d (width*depth=%d)\n",
        fb.pitch, fb.virtual_width*fb.bit_depth/8);
    uart_putchar(EOT);
}
