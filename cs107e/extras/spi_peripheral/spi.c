/*
 * Hardware SPI driver on Mango Pi
 *
 * Author: Yifan Yang <yyang29@stanford.edu>
 *
 * Date: Mar 5, 2024
 */

#include "spi.h"
#include "ccu.h"
#include "gpio.h"
#include <stdint.h>

typedef union {
    struct {
        uint32_t reserved0;
        struct {
            uint32_t spi_en: 1;
            uint32_t master_mode: 1;
            uint32_t sample_timing_mode: 1;
            uint32_t dbi_mode_sel: 1;
            uint32_t dbi_en: 1;
            uint32_t reserved0: 2;
            uint32_t transmit_pause_en: 1;
            uint32_t reserved2: 23;
            uint32_t soft_reset: 1;
        } gcr;
        struct {
            uint32_t cpha: 1;
            uint32_t cpol: 1;
            uint32_t spol: 1;
            uint32_t ssctl: 1;
            uint32_t chip_sel: 2;
            uint32_t rest: 25;
            uint32_t start_burst: 1;
        } tcr;
        uint32_t reserved1;
        uint32_t ier;
        struct {
            uint32_t rx_ready: 1;
            uint32_t rx_empty: 1;
            uint32_t rx_full: 1;
            uint32_t reserved0: 1;
            uint32_t tx_ready: 1;
            uint32_t tx_empty: 1;
            uint32_t tx_full: 1;
            uint32_t reserved1: 1;
            uint32_t rx_overflow: 1;
            uint32_t rx_underflow: 1;
            uint32_t tx_overflow: 1;
            uint32_t tx_underflow: 1;
            uint32_t transfer_complete: 1;
            uint32_t ss_invalid: 1;
            uint32_t rest: 18;
        } isr;
        uint32_t fcr;
        struct {
            uint32_t rx_fifo_cnt: 8;
            uint32_t reserved0: 4;
            uint32_t rb_cnt: 3;
            uint32_t rb_wr: 1;
            uint32_t tx_fifo_cnt: 8;
            uint32_t reserved1: 4;
            uint32_t tb_cnt: 3;
            uint32_t tb_wr: 1;
        } fsr;
        uint32_t wcr;
        uint32_t reserved2;
        uint32_t samp_dl;
        uint32_t reserved3;
        uint32_t mbc;
        uint32_t mtc;
        struct {
            uint32_t stc: 24;
            uint32_t rest: 8;
        } bcc;
        uint32_t reserved4;
        uint32_t batcr;
        uint32_t ba_ccr;
        uint32_t tbr;
        uint32_t rbr;
        uint32_t reserved5[14];
        uint32_t ndma_mode_ctl;
        uint32_t dbi[93];
        unsigned char txd[4];
        uint32_t reserved6[63];
        unsigned char rxd[4];
    } regs;
    unsigned char padding[0x304];
} spi_t;

#define SPI_BASE ((spi_t *)0x04026000)
_Static_assert(&(SPI_BASE->regs.rxd[0]) == (unsigned char *)0x04026300, "SPI1 rxd reg must be at address 0x04026300");
_Static_assert(&(SPI_BASE->regs.txd[0]) == (unsigned char *)0x04026200, "SPI1 txd reg must be at address 0x04026200");
_Static_assert(&(SPI_BASE->regs.ndma_mode_ctl) == (uint32_t *)0x04026088, "SPI1 ndma_mode_ctl reg must be at address 0x04026088");

volatile spi_t *module = ((spi_t *)0x04026000);

void
spi_init (void)
{
    const uint32_t SPI1_CLK_REG = 0x0944;
    const uint32_t SPI_CLK_REG_VAL = (1 << 31) | (0b01 << 8) | (11);
    ccu_write (SPI1_CLK_REG, SPI_CLK_REG_VAL);
    ccu_enable_bus_clk (0x096C, (1 << 1), (1 << 17));
    module->regs.gcr.soft_reset = 1;
    while (module->regs.gcr.soft_reset)
        ;
    gpio_set_function (GPIO_PD11, GPIO_FN_ALT4); // SPI1_CLK
    gpio_set_function (GPIO_PD10, GPIO_FN_ALT4); // SPI1_CS0
    gpio_set_function (GPIO_PD12, GPIO_FN_ALT4); // SPI1_MOSI
    gpio_set_function (GPIO_PD13, GPIO_FN_ALT4); // SPI1_MISO

    module->regs.gcr.spi_en = 1;
    module->regs.gcr.master_mode = 1;
}

void
spi_transfer (unsigned char *tx, unsigned char *rx, int len)
{
    module->regs.tcr.chip_sel = 0;
    module->regs.mbc = len;
    module->regs.mtc = len;
    module->regs.bcc.stc = len;
    for (unsigned i = 0; i < len; i++) {
        module->regs.txd[0] = tx[i];
        while (!module->regs.isr.tx_ready)
            ;
    }
    module->regs.tcr.start_burst = 1;
    module->regs.isr.transfer_complete = 1; // clear the transfer complete flag
    while (!module->regs.isr.transfer_complete)
      ;
    for (unsigned i = 0; i < len; i++) {
        if (module->regs.fsr.rx_fifo_cnt > 0) {
            rx[i] = module->regs.rxd[0];
        } else {
            rx[i] = 0;
        }
    }
}