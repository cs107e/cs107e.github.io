/*
* Use spi to drive rgb led strip
* Leds are DotStar APA102, strip contains 144 leds
* To control strip, send data sequence consisting of
*    start frame + n pixels + end frame
*/

#include "dma.h"
#include "spi.h"
#include "strings.h"
#include "timer.h"

// brightness range: 0 (off) to 31 (crazy bright)
#define DEFAULT_BRIGHTNESS 10

typedef struct {
    uint32_t bright : 5;
    uint32_t header : 3; // constant 0b111
    uint32_t blue   : 8;
    uint32_t green  : 8;
    uint32_t red    : 8;
} led_t;

#define COLOR(r, g, b) ((led_t){.header = 0b111, .bright= DEFAULT_BRIGHTNESS, .red = (r), .green = (g), .blue = (b)})

static void show_strip(spi_device_t *dev, led_t *pixels, int n) {
    int n_start = 4;
    int n_end = (n/2)/8 + 1; // half-bit per pixel
    int n_total = n_start + n*sizeof(led_t) + n_end;
    uint8_t strip_data[n_total];

    memset(&strip_data[0], 0, n_start);                     // start frame
    memcpy(&strip_data[4], pixels, n*sizeof(led_t));        // pixel data
    memset(&strip_data[4 + n*sizeof(led_t)], 0xff, n_end);  // end frame

    spi_write(dev, strip_data, sizeof(strip_data));
}

static void chase(spi_device_t *dev, int nleds) {
    const led_t off = COLOR(0,0,0), orange = COLOR(0xff,0x40,0), blue = COLOR(0,0,0x40);
    led_t strip[nleds];
    for (int i = 0; i < nleds; i++) strip[i] = off;
    show_strip(dev, strip, nleds);

    while (1) {
        for (int i = 0; i < nleds; i++) {
            strip[i] = orange;
            strip[nleds - 1 - i] = blue;
            show_strip(dev, strip, nleds);
            timer_delay_ms(10);
            strip[i] = strip[nleds - 1 - i] = off;
        }
    }
}

void main(void) {
    int nleds = 36;
    dma_init();
    spi_init();
    spi_device_t *dev = spi_new(GPIO_PD10, SPI_MODE_0, 1*1000*1000);
    chase(dev, nleds);
}
