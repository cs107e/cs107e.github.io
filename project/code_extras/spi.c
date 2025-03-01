/*
 * Hardware SPI driver on Mango Pi
 *
 * Author: Yifan Yang <yyang29@stanford.edu>
 *
 * Date: Mar 5, 2024
 */

#include "spi.h"
#include "assert.h"
#include "ccu.h"
#include "gpio.h"
#include <stdint.h>
#include <stddef.h>

typedef union {
    struct {
        uint32_t reserved0;
        struct {
            uint32_t spi_en         : 1;
            uint32_t master_mode    : 1;
            uint32_t sample_t_mode  : 1;
            uint32_t dbi_mode_sel   : 1;
            uint32_t dbi_en         : 1;
            uint32_t                : 2;
            uint32_t tx_pause_en    : 1;
            uint32_t                : 23;
            uint32_t soft_reset     : 1;
        } gcr;
        struct {
            uint32_t cpha           : 1;
            uint32_t cpol           : 1;
            uint32_t spol           : 1;
            uint32_t ssctl          : 1;
            uint32_t chip_sel       : 2;
            uint32_t                : 25;
            uint32_t start_burst    : 1;
        } tcr;
        uint32_t reserved1;
        uint32_t ier;
        struct {
            uint32_t rx_ready       : 1;
            uint32_t rx_empty       : 1;
            uint32_t rx_full        : 1;
            uint32_t                : 1;
            uint32_t tx_ready       : 1;
            uint32_t tx_empty       : 1;
            uint32_t tx_full        : 1;
            uint32_t                : 1;
            uint32_t rx_overflow    : 1;
            uint32_t rx_underflow   : 1;
            uint32_t tx_overflow    : 1;
            uint32_t tx_underflow   : 1;
            uint32_t tx_complete    : 1;
            uint32_t ss_invalid     : 1;
            uint32_t                : 18;
        } isr;
        uint32_t fcr;
        struct {
            uint32_t rx_fifo_cnt    : 8;
            uint32_t                : 4;
            uint32_t rb_cnt         : 3;
            uint32_t rb_wr          : 1;
            uint32_t tx_fifo_cnt    : 8;
            uint32_t                : 4;
            uint32_t tb_cnt         : 3;
            uint32_t tb_wr          : 1;
        } fsr;
        uint32_t wcr;
        uint32_t reserved2;
        uint32_t samp_dl;
        uint32_t reserved3;
        uint32_t mbc;
        uint32_t mtc;
        struct {
            uint32_t stc    : 24;
            uint32_t        : 8;
        } bcc;
        uint32_t reserved4;
        uint32_t batcr;
        uint32_t ba_ccr;
        uint32_t tbr;
        uint32_t rbr;
        uint32_t reserved5[14];
        uint32_t ndma_mode_ctl;
        uint32_t dbi[93];
        uint8_t  txd[4];
        uint32_t reserved6[63];
        uint8_t  rxd[4];
    } regs;
    uint8_t padding[0x1000];
} spi_t;

#define SPI_BASE ((spi_t *)0x04025000)
_Static_assert(&(SPI_BASE[1].regs.rxd[0]) ==  (uint8_t *)0x04026300, "SPI1 rxd reg must be at address 0x04026300");
_Static_assert(&(SPI_BASE[1].regs.txd[0]) ==  (uint8_t *)0x04026200, "SPI1 txd reg must be at address 0x04026200");
_Static_assert(&(SPI_BASE[0].regs.ier)    == (uint32_t *)0x04025010, "SPI0 rbr reg must be at address 0x04025010");

static struct {
    volatile spi_t * const spi_base, *spi;
    const gpio_id_t clock, mosi, miso, cs0;
    const unsigned int fn_spi;
} module = {
    .spi_base = &SPI_BASE[0],
    .clock  =  GPIO_PD11,
    .mosi   =  GPIO_PD12,
    .miso   =  GPIO_PD13,
    .cs0    =  GPIO_PD10,
    .fn_spi =  GPIO_FN_ALT4,
    .spi = NULL, // points to spi once initialized
};


void spi_init (spi_mode_t mode) {
    // this driver code supports only SPI 1
    module.spi = &module.spi_base[1];
    ccu_config_module_clock_rate(CCU_SPI1_CLK_REG, PARENT_HOSC, 2000000); // config for 2 MHz
    ccu_ungate_bus_clock_bits(CCU_SPI_BGR_REG, (1 << 1), (1 << 17));
    module.spi->regs.gcr.soft_reset = 1;
    while (module.spi->regs.gcr.soft_reset)
        ;
    gpio_set_function(module.clock, module.fn_spi);
    gpio_set_function(module.cs0, module.fn_spi);
    gpio_set_function(module.mosi, module.fn_spi);
    gpio_set_function(module.miso, module.fn_spi);

    int cpol = 0, cpha = 0; // set phase and polarity according to mode
    switch (mode) {
        case SPI_MODE_0: cpol = 0, cpha = 0; break;
        case SPI_MODE_1: cpol = 0, cpha = 1; break;
        case SPI_MODE_2: cpol = 1, cpha = 0; break;
        case SPI_MODE_3: cpol = 1, cpha = 1; break;
        default: error("Invalid spi mode\n");
    }
    module.spi->regs.tcr.cpol = cpol;
    module.spi->regs.tcr.cpha = cpha;

    module.spi->regs.gcr.spi_en = 1;
    module.spi->regs.gcr.master_mode = 1;
}

void spi_transfer(uint8_t *tx, uint8_t *rx, int len) {
    if (module.spi == NULL) error("spi_init() has not been called!\n");
    module.spi->regs.tcr.chip_sel = 0; // Note: assumes select device 0
    for (int i_tx = 0, i_rx = 0; i_tx < len && i_rx < len; /* advance in loop */) {
        int n_batch = 0;
        module.spi->regs.isr.tx_full = 1; // write 1 to clear flag
        while (i_tx < len && module.spi->regs.isr.tx_ready && !module.spi->regs.isr.tx_full) {
            module.spi->regs.txd[0] = tx[i_tx++]; // enqueue tx fifo until full/end of data
            n_batch++;
        }
        module.spi->regs.mbc = n_batch;
        module.spi->regs.mtc = n_batch;
        module.spi->regs.bcc.stc = n_batch;

        module.spi->regs.tcr.start_burst = 1;
        module.spi->regs.isr.tx_complete = 1; // write 1 to clear flag
        while (!module.spi->regs.isr.tx_complete) // wait til complete
          ;
        while (i_rx < len && module.spi->regs.isr.rx_ready && module.spi->regs.fsr.rx_fifo_cnt > 0) {
            rx[i_rx++] = module.spi->regs.rxd[0]; // dequeue rx fifo
        }
    }
}
