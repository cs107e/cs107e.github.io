#include "gpio.h"
#include "i2s.h"
#include "audio.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"
#include "dma.h"
// #include "sound-headers/sin16-440hz.h"
// #include "sound-headers/sin16-880hz.h"
// #include "sound-headers/welcome.h"
// #include "sound-headers/moon.h"
// #include "sound-headers/THX.h"
#include "sound-headers/shall-we-play-a-game.h"

void main ()
{
    gpio_init();
    uart_init();
    i2s_init();
    CHANNEL_TYPE ct = pcm_metadata.mono ? MONO : STEREO;
    audio_init(pcm_metadata.sample_freq, pcm_metadata.block_alignment, ct);

    printf("starting play\n");
    audio_write_i16_dma((uint16_t *)pcm_data, sizeof(pcm_data)/sizeof(pcm_data[0]), pcm_metadata.repeat);

    int count = 0;

    while (1) {
        printf("doing other stuff count: %d\n", count); 
        count++;
        if (dma_complete(0)) {
            dma_disable(0);
            printf("Sound finished!\n");
            return;
        }
    }
}
