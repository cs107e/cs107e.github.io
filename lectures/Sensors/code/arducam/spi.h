#ifndef SPI_H
#define SPI_H

void spi_init(unsigned pol, unsigned pha, unsigned clk_div);
void spi_txrx(unsigned char* txbuf, unsigned char* rxbuf, unsigned len);

#endif