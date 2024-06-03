#include "fb.h"
#include "gl.h"
#include "printf.h"
#include "uart.h"
#include "i2s.h"
#include "audio.h"
#include "dma.h"
#include "timer.h"
#include "saturn-v-launchpad.h"
#include "saturn-v-rocket.h"
#include "apollo11-liftoff.h"

void draw_pixel(int x, int y, color_t c) {
    unsigned int per_row = fb_get_width();
    unsigned int (*im)[per_row] = fb_get_draw_buffer();
    im[y][x] = c;
}

void draw_hline(int y, color_t c) {
    for (int x = 0; x < fb_get_width(); x++) {
        draw_pixel(x, y, c);
    }
}

void draw_vline(int x, color_t c) {
    for (int y = 0; y < fb_get_height(); y++) {
        draw_pixel(x, y, c);
    }
}

void draw_launchpad() {
    for (int i = 0; i < sizeof(saturnVlaunchpadData); i += 4) {
        color_t pixel = saturnVlaunchpadData[i] << 24 | // alpha
            saturnVlaunchpadData[i + 1] << 16 | // red
            saturnVlaunchpadData[i + 2] << 8 |  // green
            saturnVlaunchpadData[i + 3];        // blue
        int col = (i / 4) % saturnVlaunchpad_WIDTH;
        int row = (i / 4) / saturnVlaunchpad_WIDTH;
        draw_pixel(col, row, pixel);
    }
}

void draw_rocket(int col_offset, int row_offset) {
    color_t alpha_color = 0xffff00ff; // magenta
    for (int i = 0; i < sizeof(rocketData); i += 4) {
        color_t pixel = rocketData[i] << 24 | // alpha
            rocketData[i + 1] << 16 | // red
            rocketData[i + 2] << 8 |  // green
            rocketData[i + 3];        // blue
        int col = (i / 4) % rocket_WIDTH + col_offset;
        int row = (i / 4) / rocket_WIDTH + row_offset;
        if (pixel != alpha_color && row >= 0) {
            draw_pixel(col, row, pixel);
        }
    }
}

void clear_rocket(int col_offset, int row_offset) {
    color_t alpha_color = 0xffff00ff; // magenta
    for (int i = 0; i < sizeof(rocketData); i += 4) {
        color_t pixel = rocketData[i] << 24 | // alpha
            rocketData[i + 1] << 16 | // red
            rocketData[i + 2] << 8 |  // green
            rocketData[i + 3];        // blue
        int col = (i / 4) % rocket_WIDTH + col_offset;
        int row = (i / 4) / rocket_WIDTH + row_offset;
        if (row >= 0) {
            int translated_i = (row * saturnVlaunchpad_WIDTH + col) * 4; 
            color_t launchpad_pixel = saturnVlaunchpadData[translated_i] << 24 | // alpha
                saturnVlaunchpadData[translated_i + 1] << 16 | // red
                saturnVlaunchpadData[translated_i + 2] << 8 |  // green
                saturnVlaunchpadData[translated_i + 3];        // blue
            if (pixel != alpha_color) {
                draw_pixel(col, row, launchpad_pixel);
            }
        }
    }
}

void start_audio() {
    i2s_init();
    CHANNEL_TYPE ct = pcm_metadata.mono ? MONO : STEREO;
    audio_init(pcm_metadata.sample_freq, pcm_metadata.block_alignment, ct);

    printf("starting audio\n");
    audio_write_i16_dma((int16_t *)pcm_data, sizeof(pcm_data)/sizeof(pcm_data[0]), pcm_metadata.repeat);
}

void main(void)  {
    uart_init();
    fb_init(saturnVlaunchpad_WIDTH, saturnVlaunchpad_HEIGHT, FB_DOUBLEBUFFER);
    draw_launchpad();
    draw_rocket(850, 180);
    gl_swap_buffer();
    start_audio();
    draw_launchpad();
    timer_delay_ms(8500);
    const int DY = 8;
    for (int row_offset = 180 - DY; row_offset >= -(rocket_HEIGHT + DY); row_offset -= DY) {
        draw_rocket(850, row_offset);
        gl_swap_buffer();
        clear_rocket(850, row_offset + DY);
    }
    while (1) {
        if (dma_complete()) {
            dma_disable();
            break;
        }
    }

    /*
       color_t blue = 0x0000ff;
       for (int y = 0; y < fb_get_height(); y += 25) {
       draw_hline(y, blue);
       }

       color_t red = 0xff0000;
       for (int x = 0; x < fb_get_width(); x += 25) {
       draw_vline(x, red);
       }
       */
    printf("Hit any key to quit: ");
    uart_getchar();
    printf("\nCompleted %s\n", __FILE__);
    while (1) {}
}
