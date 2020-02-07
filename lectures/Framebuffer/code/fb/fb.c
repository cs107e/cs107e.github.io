#include "gpio.h"
#include "uart.h"
#include "mailbox.h"
#include "printf.h"

#define GPU_NOCACHE 0x40000000

typedef struct {
    unsigned int width;       // width of the display
    unsigned int height;      // height of the display
    unsigned int virtual_width;  // width of the virtual framebuffer
    unsigned int virtual_height; // height of the virtual framebuffer
    unsigned int pitch;       // number of bytes per row
    unsigned int depth;       // number of bits per pixel
    unsigned int x_offset;    // x of the upper left corner of the virtual fb
    unsigned int y_offset;    // y of the upper left corner of the virtual fb
    unsigned int framebuffer; // pointer to the start of the framebuffer
    unsigned int size;        // number of bytes in the framebuffer
} fb_config_t;

static volatile fb_config_t fb __attribute__((aligned(16)));

const int WIDTH = 640;
const int HEIGHT = 480;
const int DEPTH = 32;

int fb_init(void) {
    // write these values to the GPU
    fb.width = fb.virtual_width = WIDTH;
    fb.height = fb.virtual_height = HEIGHT;
    fb.depth = DEPTH;
    fb.x_offset = 0;
    fb.y_offset = 0;

    // read these values from the GPU - must be 0 initially
    fb.framebuffer = 0;
    fb.size = 0;
    fb.pitch = 0;

    // send the request to the GPU using "mail"
    mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb + GPU_NOCACHE);
    int err =  mailbox_read(MAILBOX_FRAMEBUFFER);
    return err;
}

void main(void) {
    printf("requested physical size = (%d, %d)\n", WIDTH, HEIGHT);
    printf("requested virtual size = (%d, %d)\n", WIDTH, HEIGHT);
    printf("requested depth (bits) = %d\n", DEPTH);

    int err = fb_init();

    printf("err code = %x\n", err);
    printf("physical size = (%d, %d)\n", fb.width, fb.height);
    printf("virtual size = (%d, %d)\n", fb.virtual_width, fb.virtual_height);
    printf("depth (bits) = %d\n", fb.depth);
    printf("framebuffer pointer = %p\n", (unsigned int*) fb.framebuffer);
    printf("framebuffer size = %d (height*width*depth=%d)\n",
        fb.size, fb.height*fb.width*fb.depth/8);
    printf("pitch (number of bytes in a scanline)= %d (width*depth=%d)\n",
        fb.pitch, fb.width*fb.depth/8);
}
