#include "gpio.h"
#include "spi.h"

struct spi {
    unsigned cs; /* SPI Master Control and Status */
    unsigned fifo;
    unsigned clk;
    unsigned dlen;
    unsigned ltoh;
    unsigned dc;
};

#define SPI0_CLEAR_TX (1 << 4)
#define SPI0_CLEAR_RX (1 << 5)
#define SPI0_TRANSFER_ACTIVE (1 << 7)
#define SPI0_TRANSFER_DONE (1 << 16)
#define SPI0_RX_HAS_DATA (1 << 17)
#define SPI0_TX_CAN_ACCEPT (1 << 18)

volatile struct spi *spi = (struct spi *) 0x20204000;

void spi_init(unsigned chip_select, unsigned clock_divider) {
    gpio_set_function(GPIO_PIN7, GPIO_FUNC_ALT0); // SPI0_CE1_N
    gpio_set_function(GPIO_PIN8, GPIO_FUNC_ALT0); // SPI0_CE0_N
    gpio_set_function(GPIO_PIN9, GPIO_FUNC_ALT0); // SPI0_MISO
    gpio_set_function(GPIO_PIN10, GPIO_FUNC_ALT0); // SPI0_MOSI
    gpio_set_function(GPIO_PIN11, GPIO_FUNC_ALT0); // SPI0_SCLK

    spi->cs = 0;
    spi->cs |= SPI0_CLEAR_TX | SPI0_CLEAR_RX;

    if (chip_select == SPI_CE0) {
        spi->cs |= 0b00;
    } else if (chip_select == SPI_CE1) {
        spi->cs |= 0b01;
    }

    spi->clk = clock_divider;
}

void spi_transfer(unsigned char *tx, unsigned char *rx, unsigned len) {
    spi->cs |= SPI0_CLEAR_TX | SPI0_CLEAR_RX | SPI0_TRANSFER_ACTIVE;

    for (int i = 0; i < len; i++) {
        while (!(spi->cs & SPI0_TX_CAN_ACCEPT));
        spi->fifo = tx[i];

        while (!(spi->cs & SPI0_TRANSFER_DONE));
        while (!(spi->cs & SPI0_RX_HAS_DATA));
        rx[i] = (unsigned char) spi->fifo;
    }

    spi->cs &= ~SPI0_TRANSFER_ACTIVE;
}
