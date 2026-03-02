#pragma once
/*
 * File: spi.h
 *
 * Description: Module of functions that support communication
 * with devices over SPI bus.
 *
 * Author: Daniel James <drjames@stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "gpio.h"
#include <stdint.h>
/*
 * Type: `spi_mode_t`
 *
 * `spi_mode_t` is an enumerated type for the device sampling mode
 */
typedef enum {
    SPI_MODE_0 = 0,     // CPOL = 0, CPHA = 0 (Clock Low, Data Captured on Rising Edge)
    SPI_MODE_1,         // CPOL = 0, CPHA = 1 (Clock Low, Data Captured on Falling Edge)
    SPI_MODE_2,         // CPOL = 1, CPHA = 0 (Clock High, Data Captured on Falling Edge)
    SPI_MODE_3,         // CPOL = 1, CPHA = 1 (Clock High, Data Captured on Rising Edge)
} spi_mode_t;


/*
 * `spi_device_t`
 *
 * This typedef gives a nickname to the struct that will be used to represent a
 * single SPI device. The internal details of the struct will be given in the file
 * spi.c; those details are private to the implementation and are not shared in the
 * public interface. Clients of the SPI module are not privy to the details of
 * `spi_device_t`, nor should they be. A client simply holds on to the pointer returned
 * by `spi_new` and sends that pointer to the functions below that communicate with
 * the device over the SPI protocol.
 */
typedef struct spi_device spi_device_t;


/*
 * `spi_init` : Required initialization for SPI module
 *
 * Initializes the SPI module. Must run before communicating with any SPI
 * devices.
 *
 * Only need to call `spi_init` once -- subsequent calls reinitialize the module.
 */
void spi_init(void);

/*
 * `spi_new` : Create a new SPI device
 *
 * Creates a new SPI device. The given gpio pin is activated as chip select before
 * any transmissions. Sampling for this device uses the specified mode and
 * operates at speed specified by rate (expressed in bits per second). Choose
 * the mode and speed according to device requirements. Returns pointer to newly created SPI device.
 *
 * @param chip_select      gpio pin connect to the device select
 * @param mode             SPI mode for sampling
 * @param rate             sampling rate
 */
spi_device_t * spi_new(gpio_id_t chip_select, spi_mode_t mode, long rate);

/*
 * `spi_write_and_read` : Transmission will write and read at same time
 *
 * Transmit tx_len bytes from tx while reading rx_len bytes into rx.
 * If either length is zero, the corresponding buffer may be NULL.
 *
 * @param dev       pointer to target SPI device
 * @param tx        buffer of bytes to transmit
 * @param tx_len    number of bytes in transmit buffer
 * @param rx        buffer of received bytes
 * @param rx_len    number of bytes in receive buffer
 */
void spi_write_and_read(spi_device_t *dev, const uint8_t *tx, int tx_len, uint8_t *rx, int rx_len);

/*
 * `spi_write_then_read` : Transmission will write, followed by read
 *
 * First transmit tx_len bytes from tx, then receive rx_len bytes into rx.
 * If either length is zero, the corresponding buffer may be NULL.
 *
 * @param dev       pointer to target SPI device
 * @param tx        buffer of bytes to transmit
 * @param tx_len    number of bytes in transmit buffer
 * @param rx        buffer of received bytes
 * @param rx_len    number of bytes in receive buffer
 */
void spi_write_then_read(spi_device_t *dev, const uint8_t *tx, int tx_len, uint8_t *rx, int rx_len);

/*
 * `spi_write` : Transmission will write, no read
 *
 * Transmit `len` bytes from buffer `tx` to device. Does not receive any data.
 *
 * @param dev       pointer to target SPI device
 * @param tx        buffer of bytes to transmit
 * @param tx_len    number of bytes in transmit buffer
 */
void spi_write(spi_device_t *dev, const uint8_t *tx, int tx_len);
