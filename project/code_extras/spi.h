#ifndef SPI_H__
#define SPI_H__

/*
 * Author: Yifan Yang <yyang29@stanford.edu>
 *
 * Date: Mar 5, 2024
 */

#include <stdint.h>

typedef enum {
    SPI_MODE_0 = 0,     // CPOL = 0, CPHA = 0 (Clock Low, Data Captured on Rising Edge)
    SPI_MODE_1,         // CPOL = 0, CPHA = 1 (Clock Low, Data Captured on Falling Edge)
    SPI_MODE_2,         // CPOL = 1, CPHA = 0 (Clock High, Data Captured on Falling Edge)
    SPI_MODE_3,         // CPOL = 1, CPHA = 1 (Clock High, Data Captured on Rising Edge)
} spi_mode_t;

void spi_init(spi_mode_t mode); // once to init spi module

// SPI transfer is bi-drectional, same number of bytes transmit as receive
void spi_transfer(uint8_t *tx, uint8_t *rx, int len);

#endif