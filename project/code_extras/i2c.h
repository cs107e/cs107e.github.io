#ifndef I2C_H__
#define I2C_H__

/*
    Module to support i2c communication via TWI hardware peripheral

    Author: Julie Zelenski
 */

#include <stdbool.h>
#include <stdint.h>

typedef struct i2c_device i2c_device_t;

void i2c_init(void); // call once to init i2c module
i2c_device_t * i2c_new(uint8_t addr); // per each i2c device

bool i2c_write_reg(i2c_device_t *dev, uint8_t reg, uint8_t val);
bool i2c_write_reg_n(i2c_device_t *dev, uint8_t reg, uint8_t *bytes, int n);

uint8_t i2c_read_reg(i2c_device_t *dev, uint8_t reg);
bool i2c_read_reg_n(i2c_device_t *dev, uint8_t reg, uint8_t *bytes, int n);

// if register specifier not single byte, block read/write to manually construct packet
bool i2c_block_read(i2c_device_t *dev, uint8_t *bytes, int n);
bool i2c_block_write(i2c_device_t *dev, uint8_t *bytes, int n);

#endif