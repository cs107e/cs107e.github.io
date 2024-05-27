#ifndef I2S_H
#define I2S_H

#include<stdint.h>

/*
 * Hardware abstractions for pulse width modulation (I2S) of
 * digital signals.
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Date: Feb 15, 2016
 */

/*
 * Initialize the i2s module. 
 */
void i2s_init(void);

/*
 * Set the I2S clock frequency.
 *
 * @param frequency      the frequency of the I2S clock
*/
void i2s_set_clock(int frequency);

/*
 * Setup I2S
 */
void i2s_setup(int frequency, int block_alignment);

/*
 * Set the I2S mode.
 *
 * @param chan      the I2S channel - either 0 or 1
 * @param markspace use I2S_MARKSPACE mode or I2S_SIGMADELTA mode
 */
#define I2S_SIGMADELTA 0
#define I2S_MARKSPACE  1
void i2s_set_mode(int chan, int markspace);

/*
 * Set the I2S fifo..
 *
 * @param chan      the I2S channel - either 0 or 1
 * @param usefifo   usefifo or not
 */
void i2s_set_fifo(int chan, int usefifo);
void i2s_clear_fifo(void);

/*
 * Enable or disable I2S.
 *
 * @param chan      the I2S channel - either 0 or 1
 */
void i2s_enable();
void i2s_disable(int chan);

/*
 * Set the I2S range for the given channel
 *
 * @param chan      the I2S channel - either 0 or 1
 * @param range     range (typically between 0 and 4095)
 */
void i2s_set_range(int chan, int range);

/*
 * Set the I2S width for the given channel
 *
 * @param chan      the I2S channel - either 0 or 1
 * @param width     width (typically between 0 and 4095). A width of range/2
 *                  corresponds to a 50% duty cycle.
 */
void i2s_set_width(int chan, int width);

/*
 * Write a value to all channels
 *
 * @param value     value (typically between 0 and 4095). Set the I2S width 
 *                  to value.
 */
void i2s_write(uint16_t left, uint16_t right);

/*
 * Read the current status of the I2S channel.
 *
 * @return the current status of the channels. If I2S_FULL1 is set, then
 * need to wait until the channel is not full before writing a new value.
 */
#define I2S_FULL1 0x01
unsigned int i2s_get_status(void);

#endif
