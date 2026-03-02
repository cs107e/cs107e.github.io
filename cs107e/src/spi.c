/*
 * File: spi.c
 *
 *  SPI implementation
 *
 * Most of these functions are simple wrappers that pass through to spi_driver
 * to access the hardware. The standard interface only supports blocking transactions
 * (simple and safe), but internally all are implemented as a nb transfer
 * followed by a synchronous wait.
 *
 * Author: Daniel James <drjames@stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 *
 * Last updated: Curis summer 2025
 */

#include "spi.h"
#include "spi_driver.h"
#include "spi_extra.h"
#include <stddef.h>

void spi_init(void) {
    spid_init();
}

spi_device_t *spi_new(gpio_id_t chip_select, spi_mode_t mode, long rate) {
    return spid_new_device(chip_select, mode, rate);
}

void spi_write_and_read(spi_device_t *dev, const uint8_t *tx, int tx_len, uint8_t *rx, int rx_len) {
    spid_transfer_nb(dev, tx, tx_len, rx, rx_len, false);
    spi_await_transfer_completed(dev);
}

void spi_write_then_read(spi_device_t *dev, const uint8_t *tx, int tx_len, uint8_t *rx, int rx_len) {
    spid_transfer_nb(dev, tx, tx_len, rx, rx_len, true);
    spi_await_transfer_completed(dev);
}

void spi_write(spi_device_t *dev, const uint8_t *tx, int len) {
    spi_write_then_read(dev, tx, len, NULL, 0);
}

void spi_write_and_read_nb(spi_device_t *dev, const uint8_t *tx, int tx_len, uint8_t *rx, int rx_len) {
    spid_transfer_nb(dev, tx, tx_len, rx, rx_len, false);
}
void spi_write_then_read_nb(spi_device_t *dev, const uint8_t *tx, int tx_len, uint8_t *rx, int rx_len) {
    spid_transfer_nb(dev, tx, tx_len, rx, rx_len, true);
}
void spi_write_nb(spi_device_t *dev, const uint8_t *tx, int len) {
    spid_transfer_nb(dev, tx, len, NULL, 0, true);
}

bool spi_transfer_completed(spi_device_t *dev) {
    return spid_transfer_completed(dev);
}
void spi_await_transfer_completed(spi_device_t *dev) {
    while (!spid_transfer_completed(dev));
}

bool spi_is_bus_free(void) {
    return spid_is_bus_free();
}
void spi_await_bus_free(void) {
    while (!spid_is_bus_free());
}
