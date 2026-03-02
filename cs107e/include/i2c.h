#pragma once
/*
 * File: i2c.h
 *
 * Description: Module to support communication with devices
 * over I2C bus.
 *
 * Author: Elias Chikwanda <eliaschi@stanford.edu>
 * Author: Joe Robertazzi  <tazzi@stanford.edu>
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * Type: `i2c_clck_freq_t`
 *
 * `i2c_clk_freq_t` is an enumerated type used to refer to the clock frequency
 * of the I2C module. Values can be I2C_25KHZ`, `I2C_100KHZ`, or `I2C_400KHZ.
 * Speed is the same for all devices on shared bus.
 */
typedef enum {
    I2C_100KHZ = 0,
    I2C_400KHZ = 1,
    I2C_20KHZ =  2,
} i2c_clk_freq_t;

/*
 * Type: `i2c_reg_size_t`
 *
 * `i2c_reg_size_t` is an enumerated type used to refer to the size of
 * the register for an I2C device. Values can be `I2C_REG_8BIT` or `I2C_REG_16BIT`.
 * This setting is per-device.
 */
typedef enum {
    I2C_REG_8BIT =  1,
    I2C_REG_16BIT = 2,
} i2c_reg_size_t;

/*
 * Type: `i2c_transition_t`
 *
 * `i2c_transition_t` is an enumerated type that refers to how the device should
 * transition from write to read in a transaction that read from a register.
 * Values can be `I2C_REPEATED_START` or `I2C_STOP_START`.
 * This setting is per-device.
 */
typedef enum {
    I2C_REPEATED_START =  0,
    I2C_STOP_START = 1,
} i2c_transition_t;

#define I2C_NO_DELAY 0

/*
 * `i2c_device_t`
 *
 * This typedef gives a nickname to the struct that will be used to represent a
 * single I2C device. The internal details of the struct will be given in the file
 * i2c.c; those details are private to the implementation and are not shared in the
 * public interface. Clients of the I2C module are not privy to the details of
 * `i2c_device_t`, nor should they be. A client simply holds on to the pointer returned
 * by `i2c_new` and sends that pointer to any of the functions below reading from and
 * writing to the device over the I2C protocol.
 */
typedef struct i2c_device i2c_device_t;

/*
 * `i2c_init` : Required initialization for I2C module
 *
 * Initializes the I2C module. Must run before communicating with any I2C
 * devices. Valid clock rates are `I2C_20KHZ`, `I2C_100KHZ`, and `I2C_400KHZ`.
 * The pins `GPIO_PG12` and `GPIO_PG13` are re-configured for use as I2C SCL and SDA.
 *
 * @param rate      desired clock rate of the I2C module (see options above)
 *
 * Only need to call `i2c_init` once -- subsequent calls reinitialize the module.
 */
void i2c_init(i2c_clk_freq_t rate);

/*
 * `i2c_new` : Create a new I2C device
 *
 * Creates a new I2C device for the given I2C address/id. Must run before attempting to read/write
 * from the device. `i2c_new` will establish communication with device at address `addr` on the bus,
 * and return a pointer to a struct containing that device's data if successful or return `NULL`
 * if the communication fails. Can alternatively be used as a scan of bus if check every possible
 * address and those that return valid pointers are valid devices.
 *
 * @param addr      address of the target I2C device
 */
i2c_device_t* i2c_new(uint8_t addr);

/*
 * `i2c_config_device_settings` : Optional configuration of I2C device settings
 *
 * Default settings for a new device are `I2C_REG_8BIT`, `I2C_REPEATED_START`, and `I2C_NO_DELAY`.
 * Call this function if needed to change settings to match requirements of your device.
 *
 * Note: `trans_delay_us` MUST be `0` if `transition` is `I2C_REPEATED_START` as there is no delay between
 * repeated starts. An assert is raised if requested configuration is invalid.
 *
 * @param dev               pointer to target I2C device
 * @param reg_size          size of registers on this device (either `I2C_REG_8BIT` or `I2C_REG_16BIT`)
 * @param transition        how to handle transition (either `I2C_REPEATED_START` or `I2C_STOP_START`)
 * @param trans_delay_us    if transition stop-start, number of microseconds to insert stop-delay-start (`0` if none)
 */
void i2c_config_device_settings(i2c_device_t* dev, i2c_reg_size_t reg_size, i2c_transition_t transition, int trans_delay_us);

/*
 * `i2c_write_reg_n` : Multi-byte register write for I2C device
 *
 * Writes `n` bytes to register `reg` for target I2C device. Returns `true` if the bytes
 * were successfully written or `false` if the write failed.
 *
 * @param dev       pointer to target I2C device
 * @param reg       register where data bytes will be written
 * @param bytes     array of data bytes to write
 * @param n         number of bytes to write
 */
bool i2c_write_reg_n(i2c_device_t* dev, uint16_t reg, const uint8_t* bytes, int n);

/*
 * `i2c_write_reg` : Single byte register write for I2C device
 *
 * Writes one byte to register `reg` for target I2C device. Returns `true` if the byte
 * was successfully written or `false` if the write failed.
 *
 * @param dev       pointer to target I2C device
 * @param reg       register where data byte will be written
 * @param val       data byte to write
 */
bool i2c_write_reg(i2c_device_t* dev, uint16_t reg, uint8_t val);

/*
 * `i2c_read_reg_n` : Multi-byte register read for I2C device
 *
 * Reads `n` bytes from register `reg` for target I2C device. Returns `true` if the bytes
 * were successfully read or `false` if the read failed.
 *
 * @param dev       pointer to target I2C device
 * @param reg       register where data bytes will be read
 * @param bytes     array to store the data bytes that are read
 * @param n         number of bytes to read
 */
bool i2c_read_reg_n(i2c_device_t* dev, uint16_t reg, uint8_t* bytes, int n);

/*
 * `i2c_read_reg` : Single byte register read for I2C device
 *
 * Reads one byte from register `reg` for target I2C device. Returns the byte read
 * from the register if read succeeded, and `-1` if the read failed.
 *
 * @param dev       pointer to target I2C device
 * @param reg       register where data byte will be read
 */
int i2c_read_reg(i2c_device_t* dev, uint16_t reg);

/*
 * `i2c_block_write` : Multi-byte write for I2C device
 *
 * Writes `n` bytes to the target I2C device. Returns `true` if the bytes
 * were successfully written or `false` if the write failed.
 *
 * @param dev       pointer to target I2C device
 * @param bytes     array of data bytes to write
 * @param n         number of bytes to write
 */
bool i2c_block_write(i2c_device_t* dev, const uint8_t* bytes, int n);

/*
 * `i2c_block_read` : Multi-byte read for I2C device
 *
 * Reads `n` bytes from the target I2C device. Returns `true` if the bytes
 * were successfully read or `false` if the read failed.
 *
 * @param dev       pointer to target I2C device
 * @param bytes     array to store the data bytes that are read
 * @param n         number of bytes to read
 */
bool i2c_block_read(i2c_device_t* dev, uint8_t* bytes, int n);
