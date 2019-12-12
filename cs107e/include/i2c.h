#ifndef I2C_H
#define I2C_H

/*
 * I2C driver for the Raspberry Pi. Use this to interface with sensors
 * and other peripherals that use the I2C protocol.
 *
 * Author: Anna Zeng <zeng@cs.stanford.edu>
 * Date: May 14, 2016
 */

void i2c_init(void);
void i2c_read(unsigned slave_address, char *data, int data_length);
void i2c_write(unsigned slave_address, char *data, int data_length);

#endif
