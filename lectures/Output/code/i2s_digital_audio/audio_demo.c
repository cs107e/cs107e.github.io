/* File: audio_demo.c
 * ------------------
 * Demo I2S playback and capture
 */
#include "assert.h"
#include "dma.h"
#include "i2s.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

#include "all-right-now.h" // header file contains array of samples converted from wav file

static void pause(const char *msg) {
    printf("%s ", msg);
    while (!uart_haschar());
    uart_putchar(uart_getchar());
}

void main(void) {
    uart_init();
    dma_init();

    assert(gwav->bits_per_sample == 16); // DAC support only tested for 16-bit
    int sample_freq = gwav->frames_per_sec;
    i2s_init(sample_freq);
    i2s_frame_type_t ftype = (gwav->samples_per_frame == 1) ? I2S_MONO : I2S_STEREO;

    i2s_stream_playback_nb(gwav->data, gwav->n_data, ftype);
    i2s_await();

    pause("enter to start capture: ");
    int n_samples = sample_freq*5;
    int16_t samples[n_samples];
    i2s_stream_capture_nb(samples, n_samples, I2S_MONO);
    while (!i2s_transfer_completed()) {
        uart_putchar('.');
        timer_delay_ms(200);
    }
    printf("done.\n");

    i2s_stream_playback_nb(samples, n_samples, I2S_MONO);
    i2s_await();
}
