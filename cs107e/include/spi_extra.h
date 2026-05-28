#pragma once
/*
 * File: spi_extra.h
 *
 * This header documents the advanced/optional features of the spi module.
 *
 * The *_nb versions of spi_write perform SPI communication in non-blocking
 * manner (asychronous) using DMA transfer. This allows CPU to perform other work while spi
 * transfer is ongoing. This can be tricky to use correctly, read the caution
 * notes below.
 *
 * The convenience macros can be handy to transmit fixed values without having
 * to create an array.
 *
 * Author: Daniel James <drjames@stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "spi.h"

// Non blocking versions of the equivalent functions from spi.h. Use
// spi_is_bus_free to determine when the transfer is finished.
//
// *DO NOT* initiate a new transfer while a non-blocking transfer is ongoing.
//
// CAUTION: Be very careful that the buffer(s) passed as argument live long
// enough for the transfer to finish. For example, a stack-allocated buffer
// is almost certainly not valid. Also do not modify contents of buffers
// while transfer is in progress.
//
void spi_write_and_read_nb(spi_device_t *dev, const uint8_t *tx, int tx_len, uint8_t *rx, int rx_len);
void spi_write_then_read_nb(spi_device_t *dev, const uint8_t *tx, int tx_len, uint8_t *rx, int rx_len);
void spi_write_nb(spi_device_t *dev, const uint8_t *tx, int len);

// Returns true once most recently initiated non-blocking transfer on this device
// is complete.
bool spi_transfer_completed(spi_device_t *dev);
// Blocking wait for spi_transfer_complete to return true.
void spi_await_transfer_completed(spi_device_t *dev);

// Similar to spi_transfer_completed but instead of checking a specific device
// it waits for the transfer to complete on whichever the most recent device
// was. 
//
// CAUTION: If you call any spi transfer function while spi_is_bus_free is
// returning false, you will get an error. Always use spi_await_bus_free or
// check spi_is_bus_free before initiating a new transaction.
bool spi_is_bus_free(void);
void spi_await_bus_free(void);


// Convenience function for calling spi_write. Instead of creating a stack
// array just write out the bytes you want to transfer as the arguments directly.
#define spi_write_v(dev, ...) do { \
    uint8_t __tx[] = { __VA_ARGS__ }; \
    spi_write(dev, __tx, sizeof(__tx)); \
} while(0)

// Convenience function for calling spi_write_and_read. Instead of creating a stack
// array just write out the bytes you want to transfer as the arguments directly.
// Note that unlike spi_write_and_read the rx buffer and len go first due to the limitations
// of C macros.
#define spi_write_and_read_v(dev, rx, rx_len, ...) do { \
    uint8_t __tx[] = { __VA_ARGS__ }; \
    spi_write_and_read(dev, __tx, sizeof(__tx), rx, rx_len); \
} while(0)

#define spi_write_then_read_v(dev, rx, rx_len, ...) do { \
    uint8_t __tx[] = { __VA_ARGS__ }; \
    spi_write_then_read(dev, __tx, sizeof(__tx), rx, rx_len); \
} while(0)
