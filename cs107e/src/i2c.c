/*
 * File: i2c.c
 *
 * Description: Contains API support for I2C devices on the MangoPi. Allows users
 * to initialize an I2C module, initialize multiple I2C devices, and read/write to
 * said devices by registers or direct access. Check i2c.h for more specification 
 * on the `i2c_reg_size_t` and `i2c_clk_freq_t` types.
 *
 * Author: Elias Chikwanda <eliaschi@stanford.edu>
 * Author: Joe Robertazzi  <tazzi@stanford.edu>
 *
 * Last updated: Curis summer 2025
 */

#include "i2c.h"
#include "twi_driver.h"
#include "assert.h"
#include "malloc.h"
#include "strings.h"
#include "timer.h"

#define SENTINEL 0x7e  // For debugging

struct i2c_device {
    uint8_t addr;
    i2c_reg_size_t reg_size;  // Valid values are `I2C_REG_8BIT` & `I2C_REG_16BIT`
    i2c_transition_t transition; // Valid values are `I2C_REPEATED_START` & `I2C_STOP_START`
    int trans_delay_us; // If transition is I2C_STOP_START and requires delay
};

// Initialize I2C module with clockspeed `rate`
void i2c_init(i2c_clk_freq_t rate) { 
    twi_init(rate); 
}

// Create I2C device with address `addr` and default device settings.
i2c_device_t* i2c_new(uint8_t addr) {
    i2c_device_t *dev = malloc(sizeof(*dev));
    dev->addr = addr;
    dev->reg_size = I2C_REG_8BIT;  // default config
    dev->transition = I2C_REPEATED_START;
    dev->trans_delay_us = I2C_NO_DELAY;

    // Try to communicate with device at addr, return NULL on failure
    if (!twi_do_transaction(dev->addr, NULL, 0, NULL, 0)) {
        free(dev);
        return NULL;
    }
    return dev;
}

// Allows the user to change I2C device register size, transition state, and transition delay in microseconds
// Note: Invalid to have a non-zero delay if I2C_REPEATED_START transition
void i2c_config_device_settings(i2c_device_t* dev, i2c_reg_size_t reg_size, i2c_transition_t transition, int trans_delay_us) {
    assert(dev);
    dev->reg_size = reg_size;
    dev->transition = transition;
    assert(!(transition == I2C_REPEATED_START && trans_delay_us != 0));   // If repeated start, there CANNOT be any transition delay (acts as a single continuous operation)
    dev->trans_delay_us = trans_delay_us;
}

bool i2c_write_reg(i2c_device_t* dev, uint16_t reg, uint8_t val) {
    assert(dev);
    uint8_t buf[1] = { val };
    return i2c_write_reg_n(dev, reg, buf, sizeof(buf));
}

bool i2c_write_reg_n(i2c_device_t* dev, uint16_t reg, const uint8_t* input_buffer, int n) {
    assert(dev);
    uint8_t buf[dev->reg_size + n];

    /*
    * Big Endian assumed -- if register is 16-bits wide, the most significant byte is assumed
    * to be the lower byte (i.e. the first byte in the register sequence) while the least 
    * significant byte is assumed to be the upper byte (the second byte in the register sequence).
    * 
    * Ex: If the 16 bit register address is 0x1234, buffer sent is broken into [0x12] [0x34] [data]
    * --> Little Endian would be swapped ([0x34] [0x12] [data])
    */
    if (dev->reg_size == I2C_REG_16BIT) buf[0] = (reg >> 8) & 0xff;
    buf[dev->reg_size - 1] = reg & 0xff;

    memcpy(buf + dev->reg_size, input_buffer, n);
    return twi_do_transaction(dev->addr, buf, sizeof(buf), NULL, 0);
}

int i2c_read_reg(i2c_device_t* dev, uint16_t reg) {
    assert(dev);
    uint8_t buf[1];
    
    // Return value if write succeeds, otherwise return -1 on failure
    if (i2c_read_reg_n(dev, reg, buf, 1)) return buf[0];
    else return -1;
}

bool i2c_read_reg_n(i2c_device_t* dev, uint16_t reg, uint8_t* output_buffer, int n) {
    assert(dev);
    memset(output_buffer, SENTINEL, n);
    uint8_t buf[dev->reg_size];

    // Refer to explanation above regarding how the registers are formatted
    if (dev->reg_size == I2C_REG_16BIT) buf[0] = (reg >> 8) & 0xff;
    buf[dev->reg_size - 1] = reg & 0xff;

    // Write to I2C device to specify which register to read from
    if (dev->transition == I2C_STOP_START) {
        if (!twi_do_transaction(dev->addr, buf, sizeof(buf), NULL, 0)) return false;
        if (dev->trans_delay_us > 0) timer_delay_us(dev->trans_delay_us);
        return twi_do_transaction(dev->addr, NULL, 0, output_buffer, n);
    } else {
        return twi_do_transaction(dev->addr, buf, sizeof(buf), output_buffer, n);
    }
}

// Wrapper for HAL block write
bool i2c_block_write(i2c_device_t* dev, const uint8_t* bytes, int n) {
    return twi_do_transaction(dev->addr, bytes, n, NULL, 0);
}

// Wrapper for HAL block read
bool i2c_block_read(i2c_device_t* dev, uint8_t* bytes, int n) {
    return twi_do_transaction(dev->addr, NULL, 0, bytes, n);
}
