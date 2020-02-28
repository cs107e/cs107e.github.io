/* 
 * This code brought to you by the rag tag duo of Arjun and Eric! Visit us at
 * our github accounts: https://github.com/arjunvb, https://github.com/efritz09
 * 
 * Completed on: March 14, 2016
 */

#include "gpio.h"
#include "gpioextra.h"
#include "spi.h"

volatile unsigned int *SPI_CS_reg = (unsigned int *) 0x20204000;
#define SPI_TXD_FULL_bm (1 << 18)
#define SPI_RXD_FULL_bm (1 << 17)
#define SPI_DONE_bm     (1 << 16)
#define SPI_REN_bm      (1 << 12)
#define SPI_TA_bm       (1 <<  7)
#define SPI_CSPOL_bm    (1 <<  6)
#define SPI_CLEAR_RX_bm (1 <<  5)
#define SPI_CLEAR_TX_bm (1 <<  4)
#define SPI_CPOL_bm     (1 <<  3)
#define SPI_CPHA_bm     (1 <<  2)
#define SPI_CS_bm       (1 <<  0)

volatile unsigned int *SPI_FIFO_reg = (unsigned int *) 0x20204004;
#define SPI_DATA_bm  (1 << 0)

volatile unsigned int *SPI_CLK_reg = (unsigned int *) 0x20204008;
#define SPI_CDIV_bm  (1 << 0)

#define CE1  GPIO_PIN7
#define CE0  GPIO_PIN8
#define MISO GPIO_PIN9
#define MOSI GPIO_PIN10
#define SCLK GPIO_PIN11

void spi_init_gpio()
{
	gpio_init();

	// set to GPIOs to SPI functionality
	gpio_set_function(CE1,  GPIO_FUNC_ALT0);
	gpio_set_function(CE0,  GPIO_FUNC_ALT0);
	gpio_set_function(MISO, GPIO_FUNC_ALT0);
	gpio_set_function(MOSI, GPIO_FUNC_ALT0);
	gpio_set_function(SCLK, GPIO_FUNC_ALT0);

	gpio_set_pullup(CE0);
	gpio_set_pullup(CE1);
}

void spi_clear_txrx()
{
	*SPI_CS_reg |= (SPI_CLEAR_TX_bm | SPI_CLEAR_RX_bm);
}

void spi_init(unsigned pol, unsigned pha, unsigned clk_div)
{
	spi_init_gpio();

	*SPI_CS_reg = 0;

	// clear tx/rx channel
	spi_clear_txrx();

	// chip select 0, chip select polarity low

	// clock phase and polarity
	if (pol) *SPI_CS_reg |= SPI_CPOL_bm; 
	if (pha) *SPI_CS_reg |= SPI_CPHA_bm;

	// clock divider
	*SPI_CLK_reg = clk_div;
}

void spi_txrx(unsigned char* txbuf, unsigned char* rxbuf, unsigned len) 
{	
	//clear the tx/rx
	spi_clear_txrx();

	// bring chip select low
	*SPI_CS_reg |= SPI_TA_bm;

	for (int i=0; i < len; i++) {
		// send byte
		*SPI_FIFO_reg = txbuf[i];

		// wait until done
		while(!(*SPI_CS_reg & SPI_DONE_bm));

		while (!(*SPI_CS_reg & SPI_RXD_FULL_bm));
		// read byte
		rxbuf[i] = *SPI_FIFO_reg;
	}

	// bring chip select back up
	*SPI_CS_reg &= ~(SPI_TA_bm);
}
