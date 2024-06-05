#include <stdint.h>
#include "timer.h"
#include "gpio.h"
#include "i2s.h"
#include "audio.h"
#include "printf.h"
#include "mango.h"
#include "malloc.h"
// #include "interrupts.h"
#include "dma.h"

/* Modified by Chris Gregg for the Mango Pi using i2s, May 2024 */

/*
   Initialize the i2s for audio.
*/

void audio_init(int sample_freq, int block_alignment, CHANNEL_TYPE channel_type) 
{
    i2s_setup(sample_freq, block_alignment);
    i2s_enable(channel_type);
    // interrupts_init();
    // interrupts_global_enable();
}

void mic_init() 
{
    i2s_mic_setup(44100);
    i2s_mic_enable();
}

/* 
   These functions transmit a wave to the RPi audio jack 
   as a pulse-width-modulated signal.
   
   The waveform array contains 256 entries.

   There are 3 functions depending on the type of the
   waveform array: u8, u16, i16.

   phase is a 32-bit quantity. The upper 8-bits are used as an
   index into the waveform array and selects one of the 256 samples.
   
   dphase is added to phase after each sample is output.  

   Setting dphase=(1<<24) will output one sample per interval. 
   Running the i2s at 9600000 / 256 samples / 256 range results
   in the waveform being output at 146.5 Hz.

   Setting dphase=(2<<24) will skip one sample per interval,
   and will double the frequency of the output to 293Hz.
   
   If repeat is true, the functions will not return.
*/

void audio_write_i16(const uint16_t waveform[], unsigned num_samples, int mono, int repeat) 
{
    i2s_start();
    while (1) {
        for (unsigned int sample = 0; sample < num_samples; sample++) {
            unsigned status = i2s_get_status();
            while (status == 0) {
                status = i2s_get_status();
            }
            if (mono) {
                uint16_t pcm = waveform[sample];
                i2s_write_mono(pcm);
            } else { // stereo
                uint16_t pcm_left = waveform[sample];
                uint16_t pcm_right = waveform[sample + 1];
                i2s_write_stereo(pcm_left, pcm_right);
                sample++;
            }
        }
        printf("repeating\n");
        if (!repeat) break;
    }
}

void audio_write_i16_stereo_mix(const uint16_t waveform1[], const uint16_t waveform2[], unsigned num_samples, int repeat) 
{
    i2s_start();
    while (1) {
        for (unsigned int sample = 0; sample < num_samples; sample++) {
            unsigned status = i2s_get_status();
            while (status == 0) {
                status = i2s_get_status();
            }
            uint16_t pcmL = waveform1[sample];
            uint16_t pcmR = waveform2[sample];
            i2s_write_stereo( pcmL, pcmR );
        }
        printf("repeating\n");
        if (!repeat) break;
    }
}

void audio_write_i16_dma(uint16_t waveform[], unsigned int num_samples, int repeat) {

    i2s_enable_interrupts();
    // create a 32-bit waveform
    uint32_t *wide_waveform = malloc(sizeof(uint32_t) * num_samples);
    // left shift all 16-bit values into upper part of the 32-bit space
    for (int i = 0; i < num_samples; i++) {
        wide_waveform[i] = ((uint32_t)waveform[i] << 16);
    }
    volatile I2S *i2s2 = (I2S *)I2S_2_BASE;
    printf("num_samples: %d\n", num_samples);
    dma_init(wide_waveform, &i2s2->regs.txfifo, num_samples * sizeof(uint32_t)); 
    i2s_start();
    dma_start();
    // i2s2->regs.i2s_int.full = 0xf0; // enable dma
    // i2s2->regs.i2s_int.interrupt.TXEI_EN = 0x1; // enable dma
    /*
    printf("waiting for transfer...\n");
    volatile struct DMA *dmac = (struct DMA *)0x03002000UL;
    printf("dmac_cfg_regn: %p, %x\n", &dmac->dmac_channel[0].dmac_cfg_regn, *(uint32_t *)(&dmac->dmac_channel[0].dmac_cfg_regn));
    printf("dmac_cur_src_regn: %p, %x\n", &dmac->dmac_channel[0].DMAC_CUR_SRC_REGN, *(uint32_t *)(&dmac->dmac_channel[0].DMAC_CUR_SRC_REGN));
    printf("dmac_cur_dest_regn: %p, %x\n", &dmac->dmac_channel[0].DMAC_CUR_DEST_REGN, *(uint32_t *)(&dmac->dmac_channel[0].DMAC_CUR_DEST_REGN));
    */
}

void mic_capture_dma(uint32_t *audio_samples, unsigned int num_samples) {
    volatile I2S *i2s2 = (I2S *)I2S_2_BASE;
    dma_mic_init(&i2s2->regs.rxfifo, audio_samples, num_samples * sizeof(uint32_t));
    i2s_mic_start();
    i2s_enable_mic_interrupts();
    dma_mic_start();
}
