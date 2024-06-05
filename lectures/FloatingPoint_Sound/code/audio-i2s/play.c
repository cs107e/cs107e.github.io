#include "gpio.h"
#include "i2s.h"
#include "audio.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"
// #include "sound-headers/sin16-440hz.h"
// #include "sound-headers/sin16-880hz.h"
// #include "sound-headers/welcome.h"
// #include "sound-headers/moon.h"
// #include "sound-headers/THX.h"
#include "sound-headers/shall-we-play-a-game.h"

struct wav_format {
   uint32_t description; // should be 'RIFF'
   uint32_t filesize; // little endian
   uint32_t wav_header; // should be 'WAVE'
   uint32_t wav_format; // should be 'fmt '
   uint32_t type_format_size; // should be 16, as in the above four fields are 16 bytes 
   uint16_t wav_type; // 0x01 is PCM
   uint16_t num_channels; // 0x01 is mono, 0x02 is stereo, etc.
   uint32_t sample_rate; // e.g., 44100, little-endian
   uint32_t rate_plus; // sample rate * bits per sample * channels / 8
   uint16_t bits_per_sample; // e.g., 16
   uint32_t data_mark; // should be 'data'
   uint32_t data_size; // little endian
   uint8_t *data; // may be 16-bit
};


void main ()
{
    gpio_init();
    uart_init();
    i2s_init();
    CHANNEL_TYPE ct = pcm_metadata.mono ? MONO : STEREO;
    audio_init(pcm_metadata.sample_freq, pcm_metadata.block_alignment, ct);

    while (1) {
        while (1) {
            printf("starting play\n");
            audio_write_i16(pcm_data, sizeof(pcm_data)/sizeof(pcm_data[0]), pcm_metadata.mono, pcm_metadata.repeat);
            break; // for testing
        }

        printf("done playing\n");
        timer_delay_ms(1000);
    }
}
