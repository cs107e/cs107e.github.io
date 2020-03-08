#include "gpio.h"
#include "mailbox.h"
#include "printf.h"
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

int fb_init(void) {
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
    mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb);

    // read the response from the GPU
    int err =  mailbox_read(MAILBOX_FRAMEBUFFER);
    return err;
}

void main(void) {
    gpio_init();
    uart_init();

    printf("requested physical size = (%d, %d)\n", 1600, 1200);
    printf("requested virtual size = (%d, %d)\n", WIDTH, HEIGHT);
    printf("requested depth (bits) = %d\n", DEPTH);

    int err = fb_init();

    printf("result/error (should be 0) = %x\n", err);
    printf("physical size = (%d, %d)\n", fb.width, fb.height);
    printf("virtual size = (%d, %d)\n", fb.virtual_width, fb.virtual_height);
    printf("depth (bits) = %d\n", fb.bit_depth);
    printf("framebuffer address = %p\n", fb.framebuffer);
    printf("framebuffer total bytes = %d (height*width*depth=%d)\n",
        fb.total_bytes, fb.height*fb.width*fb.bit_depth/8);
    printf("pitch (number of bytes in a scanline)= %d (width*depth=%d)\n",
        fb.pitch, fb.width*fb.bit_depth/8);
    uart_putchar(EOT);
}
