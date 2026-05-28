#pragma once
/*
 * File: i2s.h
 *
 * Description: Module for digital audio input and output
 * using I2S/PCM peripheral.
 *
 * Author: Chris Gregg     <cgregg@stanford.edu>
 * Author: Julie Zelenski  <zelenski@cs.stanford.edu>
 * Author: Joe Robertazzi  <tazzi@stanford.edu>
 */

#include <stdint.h>
#include "dma.h"

/*
 * Type: `i2s_frame_type_t`
 *
 * `i2s_frame_type_t` is an enumerated type used to refer to the type of frame
 * of the audio stream. Values can be `I2S_MONO` or `I2S_STEREO`.
 */
typedef enum {
    I2S_MONO = 0,
    I2S_STEREO = 1,
} i2s_frame_type_t;

/*
 * Type: `i2s_freq_t`
 *
 * `i2s_freq_t` is an enumerated type used to refer to the sample frequency
 * that the I2S device will use. Values can be `I2S_48KHZ` or `I2S_44p1KHZ`.
 * The frequency is same for all uses of I2S device. Call i2s_init again to change.
 */
typedef enum {
    I2S_48KHZ = 48000,
    I2S_44p1KHZ = 44100,
} i2s_freq_t;

/*
 * `i2s_init` : Required initialization for I2S module
 *
 * Initializes the I2S module. Must run before using playback or capture.
 * Typical values are `I2S_48KHZ` and `I2S_44p1KHZ` for the `sample_frequency`
 * parameter. Also accepts additional rates, but these are not guaranteed to work.
 *
 * @param sample_frequency   the desired sample frequency
 *
 * Only need to call `i2s_init` once -- subsequent calls reinitialize the module.
 */
void i2s_init(i2s_freq_t sample_frequency);

/*
 * `i2s_stream_playback_nb` : Non-blocking DMA implementation for I2S playback
 *
 * Takes in an array of 16-bit samples `samples` of size `nsamples` and data type `ftype`
 * (which can either be `I2S_MONO` or `I2S_STEREO`) and plays the samples through the I2S
 * device via DMA.
 *
 * CAUTION: The memory for samples array must remain valid until playback completes.
 * For example, a stack-allocated buffer is almost certainly not valid. Do not modify
 * contents of samples array while playback is in progress. Use i2s_await() to wait.
 *
 * Note: Keep mind of some of the following common issues when using I2S play stream:
 *   - ONLY input `I2S_MONO` data into `I2S_MONO` outputs (audio will break if mismatched)
 *   - ONLY input `I2S_STEREO` data into `I2S_STEREO` outputs (audio will break if mismatched)
 *   - Audio sped up or slowed down usually means mismatched input/output data
 *
 * @param samples    the array of samples to be played on the I2S device - each sample is 16 bits
 *                   and in `I2S_MONO` mode, samples are copied to the left and right channels while
 *                   in `I2S_STEREO` mode, samples alternate (i.e. [L16][R16]...[L16][R16])
 * @param nsamples   the number of samples in the `samples` array
 * @param ftype      the data type of the samples, either `I2S_MONO` or `I2S_STEREO`
 */
void i2s_stream_playback_nb(const int16_t samples[], int nsamples, i2s_frame_type_t ftype);

/*
 * `i2s_stream_capture_nb` : Non-blocking DMA implementation for I2S capture
 *
 * Takes in an array of 16-bit samples `samples` of size `nsamples` and data type `ftype`
 * (which can either be `I2S_MONO` or `I2S_STEREO`) and captures samples from the I2S
 * device via DMA.
 *
 * CAUTION: The memory for samples array must remain valid until capture completes.
 * For example, a stack-allocated buffer is almost certainly not valid. Do not modify
 * contents of samples array while playback is in progress. Use i2s_await() to wait.
 *
 * Note: Keep mind of some of the following common issues when using I2S capture:
 *   - Staticky audio: check for wiring/grounding issues, as they typically are at fault
 *   - When doing STEREO mic/input, provide high signal to L/R or SELECT pin for one mic
 *
 * @param samples    the array of samples to be played on the I2S device - each sample is 16 bits
 *                   and in `I2S_MONO` mode, samples are copied to the left and right channels while
 *                   in `I2S_STEREO` mode, samples alternate (i.e. [L16][R16]...[L16][R16])
 * @param nsamples   the number of samples in the `samples` array
 * @param ftype      the data type of the samples, either `I2S_MONO` or `I2S_STEREO`
 */
void i2s_stream_capture_nb(int16_t samples[], int nsamples, i2s_frame_type_t ftype);

/*
 * `i2s_transfer_completed` : Returns whether current I2S transfer has completed
 *
 * Blocks until the DMA transfer for the current I2S transaction has completed.
 * Works for both `i2s_play_stream_nb()` and `i2s_capture_nb()`, and can be called
 * after each to determine whether the transaction is still ongoing.
 */
bool i2s_transfer_completed(void);

/*
 * `i2s_await` : Waits until current I2S transfer completes
 *
 * Blocks until the DMA transfer for the current I2S transaction has completed.
 * Works for both `i2s_play_stream_nb()` and `i2s_capture_nb()`, and should be
 * called after each to simulate a blocking implementation as both are non-blocking
 * by default.
 */
void i2s_await(void);