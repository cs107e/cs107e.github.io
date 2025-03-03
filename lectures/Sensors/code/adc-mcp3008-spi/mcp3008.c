#include "spi.h"
#include "mcp3008.h"

void mcp3008_init(void)  {
    spi_init(SPI_MODE_0);
}

unsigned int mcp3008_read(int channel_num) {
    uint8_t tx[3], rx[3];

    // "Start bit", wakes up the ADC chip. 
    tx[0] = 1;
    // "Configuration byte", single mode + channel
    // valid channels 0-7
    tx[1] = 0x80 | ((channel_num & 0x7) << 4);
    tx[2] = 0;

    spi_transfer(tx, rx, sizeof(tx));
    // reading is 10-bit (0-1023)
    return ((rx[1] & 0x3) << 8) + rx[2];
}
