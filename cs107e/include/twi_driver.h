#pragma once
/*
 * File: twi_driver.h
 *
 * Description: HAL module for the TWI (I2C) driver.
 * Clients not expected to directly use twi_driver, instead use the i2c module
 * that layers on top.
 *
 * Author: Elias Chikwanda <eliaschi@stanford.edu>
 * Author: Joe Robertazzi <tazzi@stanford.edu>
 */

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

/*
 * `twi_init` : Required initialization for the TWI driver
 *
 * Initializes the TWI module and configures it to run at the clock rate `rate`. `twi_init`
 * should be called once to init module, a subsequent call will re-initialize. Supported
 * clock rates are I2C_20KHZ, I2C_100KHZ, and I2C_400KHZ.
 *
 * @param rate   the rate/clock frequency of the TWI module
 */
void twi_init(int rate);

/*
 * `twi_do_transaction` : process single TWI transaction
 *
 * Does one transaction of the form:
 *    <start>
 *       <addr+W><bytes_w><repeated start>
 *       <addr+R><bytes_r>
 *   <stop>
 * If no bytes to write, write portion is elided
 * If no bytes to read, read portion is elided
 *
 * Returns `true` if transaction successfully completed or `false`
 * on failure.
 *
 * @param dev_id        address of the TWI device
 * @param bytes_w       array of bytes to write
 * @param num_w         number of bytes to write
 * @param bytes_r       array to store bytes that are read
 * @param num_r         number of bytes to read
 */
bool twi_do_transaction(uint8_t dev_id, const uint8_t* bytes_w, int num_w, uint8_t* bytes_r, int num_r);
