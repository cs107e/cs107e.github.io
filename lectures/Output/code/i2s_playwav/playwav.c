/* File: playwav.c
 * ---------------
 * Example of I2S playback
 * Using PCM5102A Digital Audio I2S board
 */

#include "i2s.h"
#include "assert.h"
#include "dma.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

#include "welcome-to-machine.h" // header file defines gwav struct pointer

void main(void) {
    uart_init();
    dma_init();

    printf("Playing wavfile '%s'\n", gwav->name);
    assert(gwav->bits_per_sample == 16); // DAC support only tested for 16-bit
    i2s_init(gwav->frames_per_sec);
    i2s_frame_type_t ftype = (gwav->samples_per_frame == 1) ? I2S_MONO : I2S_STEREO;
    dma_channel_id_t ch = i2s_play_stream_dma(gwav->data, gwav->n_data, ftype);

    int count = 0;
    while (!i2s_dma_playback_complete(ch)) {
        timer_delay(1);
        printf("chilling in main loop: %d seconds\n", ++count);
    }
    printf("Playback complete!\n");
}
