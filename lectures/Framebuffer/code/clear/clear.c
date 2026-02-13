#include "fb.h"
#include "de.h"
#include "hdmi.h"
#include "malloc.h"
#include "printf.h"
#include "uart.h"
#include "strings.h"

// module-level variables
static struct {
    int width;             // count of horizontal pixels
    int height;            // count of vertical pixels
    int depth;             // num bytes per pixel
    void *framebuffer;     // address of framebuffer memory
} module;


void fb_init(int width, int height, fb_mode_t mode) {
    module.width = width;
    module.height = height;
    module.depth = 4;   // our pixels always 32-bit
    int nbytes = module.width * module.height * module.depth;
    module.framebuffer = malloc(nbytes);
    // what will be contents of newly malloc'ed memory block?

    hdmi_resolution_id_t id = hdmi_best_match(width, height);   // choose from available screen resolutions
    hdmi_init(id);
    de_init(width, height, hdmi_get_screen_width(), hdmi_get_screen_height());
    de_set_active_framebuffer(module.framebuffer);
}

static void clear_char_by_char(void) {
    uint8_t *im = module.framebuffer;
    int total_bytes = module.width * module.height * module.depth;
    // write each byte in the framebuffer
    for (int i = 0; i < total_bytes; i++) {
        *im++ = 0xff; // white
    }
}

static void clear_int_by_int_green(void) {
    unsigned int *im = module.framebuffer;
    int npixels = module.width * module.height;
    for (int i = 0; i < npixels; i++) {
        *im++ = 0xff00ff00; // green
    }
}

static void clear_2d_purple(void) {
    unsigned int (*im)[module.width] = module.framebuffer;
    for (int y = 0; y < module.height; y++) {
        for (int x = 0; x < module.width; x++) {
            im[y][x] = 0xffff00ff; // purple
        }
    }
}

static bool confirm(const char *msg) {
    printf("Next: %s. Hit any key when ready (q to quit): ", msg);
    int ch = uart_getchar();
    printf("%c\n", ch);
    return ch != 'q';
}

void main(void) {
    uart_init();

    fb_init(1920, 1080, FB_SINGLEBUFFER);

    printf("Screen size %d x %d\n", hdmi_get_screen_width(), hdmi_get_screen_height());
    printf("Framebuffer size %d x %d\n", module.width, module.height);
    clear_char_by_char();
    while (1) {
        if (!confirm("clear to white, char by char")) break;
        clear_char_by_char();

        if (!confirm("clear to green, int by int")) break;
        clear_int_by_int_green();

        if (!confirm("clear to purple, 2-D of int")) break;
        clear_2d_purple();
    }
    printf("Completed %s\n", __FILE__);
}
