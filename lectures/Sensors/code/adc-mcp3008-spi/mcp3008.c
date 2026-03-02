#include "spi.h"
#include "mcp3008.h"

static spi_device_t *spi_dev;

void mcp3008_init(void) {
    // Using spi driver from libextra, yay Curis team!
    spi_init();
    spi_dev = spi_new(GPIO_PD10, SPI_MODE_0, 1*1000*1000);
}

uint16_t mcp3008_read(int channel_num) {
    uint8_t tx[3], rx[3];

    // "Start bit" wakes up ADC chip
    tx[0] = 1;
    // "Configuration byte" = read single mode + channel number
    tx[1] = 0x80 | ((channel_num & 0x7) << 4);
    tx[2] = 0;

    spi_write_and_read(spi_dev, tx, sizeof(tx), rx, sizeof(rx));
    // 10-bit sample (0-1023), divided over two bytes
    return ((rx[1] & 0x3) << 8) + rx[2];
}
