#ifndef SPI_H
#define SPI_H

/*
 * Author: Omar Rizwan <osnr@stanford.edu>
 *
 * Date: May 9, 2016
 */

#define SPI_CE0 0
#define SPI_CE1 1

void spi_init(unsigned chip_select, unsigned clock_divider);
void spi_transfer(unsigned char *tx, unsigned char *rx, unsigned len);

#endif /* SPI_H */
