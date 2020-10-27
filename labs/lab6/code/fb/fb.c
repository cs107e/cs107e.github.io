#include "assert.h"
#include "fb.h"
#include "mailbox.h"
#include "printf.h"


typedef struct {
    unsigned int width;       // width of the physical screen
    unsigned int height;      // height of the physical screen
    unsigned int virtual_width;  // width of the virtual framebuffer
    unsigned int virtual_height; // height of the virtual framebuffer
    unsigned int pitch;       // number of bytes per row
    unsigned int bit_depth;   // number of bits per pixel
    unsigned int x_offset;    // x of the upper left corner of the virtual fb
    unsigned int y_offset;    // y of the upper left corner of the virtual fb
    void *framebuffer;        // pointer to the start of the framebuffer
    unsigned int total_bytes; // total number of bytes in the framebuffer
} fb_config_t;

// Why is fb volatile?
// Why is it aligned to a multiple of 16?
static volatile fb_config_t fb __attribute__ ((aligned(16)));

void fb_init(unsigned int width, unsigned int height, unsigned int depth_in_bytes)
{
    fb.width = width;
    fb.virtual_width = width;
    fb.height = height;
    fb.virtual_height = height;
    fb.bit_depth = depth_in_bytes * 8; // convert number of bytes to number of bits
    fb.x_offset = 0;
    fb.y_offset = 0;

    // the manual requires we to set these value to 0
    // the GPU will return new values
    fb.pitch = 0;
    fb.framebuffer = 0;
    fb.total_bytes = 0;

    // Send address of fb struct to the GPU as message
    mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned int)&fb);
    assert(mailbox_read(MAILBOX_FRAMEBUFFER) == 0); // confirm successful config

    printf("Framebuffer successfully configured.\n");
    printf("physical size = %d x %d\n", fb.width , fb.height);
    printf("virtual size = %d x %d\n", fb.virtual_width , fb.virtual_height);
    printf("depth = %d bits\n", fb.bit_depth );
    printf("pitch = %d bytes per row \n", fb.pitch );
    printf("total bytes = %d\n", fb.total_bytes );
    printf("buffer address = %p\n", fb.framebuffer );
}

