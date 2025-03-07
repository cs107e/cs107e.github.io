/*
* Use spi to drive rgb led strip
* Leds are DotStar APA102, strip contains 144 leds
* To control strip, send data sequence consisting of
*    start frame + n pixels + end frame
*
* https://cdn-shop.adafruit.com/product-files/2343/SK9822_SHIJI.pdf
*/

#include "timer.h"
#include "spi.h"
#include "strings.h"

// brightness range: 0 (off) to 31 (crazy bright)
#define DEFAULT_BRIGHTNESS 10

typedef struct {
    uint32_t bright : 5;
    uint32_t header : 3; // fixed 0b111
    uint32_t blue   : 8;
    uint32_t green  : 8;
    uint32_t red    : 8;
} led_t;

#define COLOR(r, g, b) ((led_t){.header = 0b111, .bright= DEFAULT_BRIGHTNESS, .red = (r), .green = (g), .blue = (b)})

static void show_strip(led_t *pixels, int n) {
    int n_start = 4;
    int n_end = (n/2)/8 + 1; // half-bit per pixel
    int n_total = n_start + n*sizeof(led_t) + n_end;
    uint8_t strip_data[n_total], unused[n_total];

    memset(&strip_data[0], 0, n_start);                     // start frame
    memcpy(&strip_data[4], pixels, n*sizeof(led_t));        // pixel data
    memset(&strip_data[4 + n*sizeof(led_t)], 0xff, n_end);  // end frame

    spi_transfer(strip_data, unused, sizeof(strip_data));
}

static void chase(int nleds) {
    led_t OFF = COLOR(0,0,0), C1 = COLOR(0xff,0x40,0), C2 = COLOR(0,0,0x40);
    led_t strip[nleds];
    for (int i = 0; i < nleds; i++) strip[i] = OFF;
    show_strip(strip, nleds);

    while (1) {
        for (int i = 0; i < nleds; i++) {
            strip[i] = C1;
            strip[nleds - 1 - i] = C2;
            show_strip(strip, nleds);
            timer_delay_ms(2);
            strip[i] = strip[nleds - 1 - i] = OFF;
       }
    }
}

void main(void) {
    int nleds = 144;
    spi_init(SPI_MODE_0);
    chase(nleds);
}
