#include "gpio.h"
#include "i2s.h"
#include "audio.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"
#include "malloc.h"
#include "dma.h"

void main ()
{
    gpio_init();
    uart_init();
    i2s_init();
    mic_init();

    printf("starting mic read\n");
    const int num_samples = 100000; // number of samples to collect
    uint32_t *audio_samples = malloc(num_samples * sizeof(uint32_t));
    mic_capture_dma(audio_samples, num_samples);
    dma_mic_start();

    int count = 0;

    while (1) {
        printf("doing other stuff count: %d\n", count); 
        count++;
        if (dma_complete(1)) {
            dma_disable(1);
            printf("Collection finished!\n");
            // convert to 16-bit samples
            uint16_t *converted_samples = malloc(num_samples * sizeof(uint16_t));
            for (int i = 0; i < num_samples; i++) {
                converted_samples[i] = audio_samples[i] >> 16;
            }
            free(audio_samples);
            i2s_init();

            audio_init(44100, 2, MONO);

            printf("3 second pause...\n");
            timer_delay_ms(3000);
            printf("starting play\n");
            audio_write_i16_dma(converted_samples, num_samples, 0);
            while (!dma_complete(0)) {
                printf("playing audio\n");
            }
            dma_disable(0);
            printf("done playing\n");
            free(converted_samples);
            return;
        }
    }
}
