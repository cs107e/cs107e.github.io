#pragma once
/*
 * File: spi_driver.h
 *
 * Description: HAL module for the SPI driver.
 * Clients not expected to directly use spi_driver, instead use the spi.h module
 * that layers on top.
 *
 * Author: Daniel James <drjames@stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdint.h>
#include "gpio.h"
#include "spi.h"


// Call once to init SPI peripheral
void spid_init(void);

// Create a new spi device. Transmissions on the device will be done in the
// provided mode. The given gpio pin is activated as the chip select before
// any transmissions. The device can then be used with the spid_transfer_nb
// function.
spi_device_t *spid_new_device(gpio_id_t chip_select, spi_mode_t mode, long rate);

// If serialize is false then then tx_len bytes are sent from tx and rx_len bytes
// are received into rx at the same time. If serialize is true then first the
// tx bytes are sent and the rx bytes are received afterwards. If either of the 
// lengths is zero then the corresponding buffer may be NULL.
void spid_transfer_nb(
    spi_device_t *dev, 
    const uint8_t *tx, int tx_len, 
    uint8_t *rx, int rx_len, 
    bool serialize
);

// Returns true once a transfer initiated by spid_transfer_nb is finished.
bool spid_transfer_completed(spi_device_t *dev);

// Returns if the bus is currently available to start a transaction. If you
// call spid_transfer_nb while this is false you will get an error.
bool spid_is_bus_free(void);
