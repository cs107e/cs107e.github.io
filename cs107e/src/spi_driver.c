/*
 * File: spi_driver.c
 *
 *  SPI hardware driver, including support for DMA for non-blocking transfer
 *
 * Author: Daniel James <drjames@stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 *
 * Last updated: Curis summer 2025
 */

#include "spi_driver.h"
#include "assert.h"
#include "ccu.h"
#include "dma.h"
#include "gpio.h"
#include "malloc.h"

typedef union {
    struct {
        uint32_t _resa;
        struct {
            uint32_t en             : 1;
            uint32_t master_mode_sel: 1; // 0 slave 1 master
            uint32_t timing_mode_sel: 1; // 0 old mode 1 new mode
            uint32_t                : 4;
            uint32_t tx_pause_en    : 1;
            uint32_t                :23;
            uint32_t soft_reset     : 1;
        } gcr;
        struct {
            uint32_t cpha           : 1;
            uint32_t cpol           : 1;
            uint32_t spol           : 1;
            uint32_t ssctl          : 1;
            uint32_t chip_sel       : 2; // select one of four lines
            uint32_t ssowner        : 1;
            uint32_t sslevel        : 1;
            uint32_t dhb            : 1;
            uint32_t dummy_type     : 1;
            uint32_t rapid_mode_sel : 1;
            uint32_t sdc            : 1;
            uint32_t fbs            : 1;
            uint32_t sdm            : 1;
            uint32_t sddm           : 1;
            uint32_t sdc1           : 1;
            uint32_t                :15;
            uint32_t start_burst    : 1; // xch exchange burst, autoclear when mbc = 0
        } tcr;
        uint32_t _resb;
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
            uint32_t rx_underrun    : 1;
            uint32_t tx_overflow    : 1;
            uint32_t tx_underrun    : 1;
            uint32_t tx_complete    : 1;
            uint32_t ss_invalid     : 1;
            uint32_t                :18;
        } isr;
        struct {
            uint32_t rx_trig_level  : 8;
            uint32_t rx_drq_en      : 1;
            uint32_t                : 5;
            uint32_t rx_test_en     : 1;
            uint32_t rx_fifo_rst    : 1;
            uint32_t tx_trig_level  : 8;
            uint32_t tx_drq_en      : 1;
            uint32_t                : 5;
            uint32_t tx_test_en     : 1;
            uint32_t tx_fifo_rst    : 1;
        } fcr; // fifo control
        const struct {
            uint32_t rx_fifo_cnt    : 8;
            uint32_t                : 4;
            uint32_t rx_wb_cnt      : 3;
            uint32_t rx_wb_en       : 1;
            uint32_t tx_fifo_cnt    : 8;
            uint32_t                : 4;
            uint32_t tx_wb_cnt      : 3;
            uint32_t tx_wb_en       : 1;
        } fsr; // fifo status
        uint32_t wcr; // wait clock
        uint32_t _resc;
        uint32_t sample_delay;
        uint32_t _resd;
        uint32_t mbc;
        uint32_t mtc;
        struct {
            uint32_t stc            :24; // single mode transmit counter
            uint32_t                : 8; // dual/quad mode,burst control...
        } bcc;
        uint32_t _rese;
        uint32_t batcr;
        uint32_t ba_ccr;
        uint32_t tbr;
        uint32_t rbr;
        uint32_t _resf[14];
        struct {
            uint32_t dma_wait : 4;
            uint32_t ack_mode : 1;
            uint32_t act_mode : 2;
            uint32_t          : 25;
        } ndma_mode_ctl;
        uint32_t dbi[93];
        union {
            uint8_t b8;
            uint16_t b16;
            uint32_t b32;
        } txd;
        uint32_t _resg[63];
        const union {
            uint8_t b8;
            uint16_t b16;
            uint32_t b32;
        } rxd;
    } regs;
    uint8_t padding[0x1000];
} spi_t;

#define SPI_BASE ((spi_t *)0x04025000)
_Static_assert(&(SPI_BASE[0].regs.ier)     == (void *)0x04025010, "SPI0 ier reg must be at address 0x04025010");
_Static_assert(&(SPI_BASE[1].regs.rxd.b8)  == (void *)0x04026300, "SPI1 rxd reg must be at address 0x04026300");

static struct {
    volatile spi_t * const spi_base, *spi;
    const gpio_id_t clock, mosi, miso;
    const unsigned int fn_spi;

    // keep track of whether the driver is currently in use.
    spi_device_t *transferring_device;
    int generation;
} module = {
    .spi_base = &SPI_BASE[0],
    .clock  =  GPIO_PD11,
    .mosi   =  GPIO_PD12,
    .miso   =  GPIO_PD13,
    .fn_spi =  GPIO_FN_ALT4,
    .spi = NULL, // points to spi after init()

    .transferring_device = NULL,
    .generation = 0,
};

struct spi_device {
    gpio_id_t chip_select;
    spi_mode_t mode;
    long rate;

    // keep track of the DMA transfer(s) this device is currently engaged in.
    int generation;
    bool has_rx, has_tx;
    dma_transfer_id_t trans_id_rx;
};

static void config_clock(long rate) {
    static const long HOSC_24MHZ =  24 * 1000 * 1000;
    static const long PERI_600MHZ = 600 * 1000 * 1000;
    
    if (rate == 0) rate = 10*1000*1000; // 10 Mhz default

    long set_to;
    if (rate == 0) {
        set_to = ccu_config_module_clock_rate(CCU_SPI1_CLK_REG, PARENT_HOSC, rate);
    } else if (rate <= HOSC_24MHZ && (HOSC_24MHZ % rate == 0)) {
        set_to = ccu_config_module_clock_rate(CCU_SPI1_CLK_REG, PARENT_HOSC, rate);
    } else if (rate <= PERI_600MHZ && (PERI_600MHZ % rate == 0)) {
        set_to = ccu_config_module_clock_rate(CCU_SPI1_CLK_REG, PARENT_PERI, rate);
    } else {
        error("Clock rate does not divide parent clock rates.");
    }
    long mhz = set_to/(1000*1000);
    // Timing settings below gleaned from D1 user manual p. 935
    // Empiricially verified using read from SPI flash + logic analzyer
    // Flash reliably read up to 120 Mhz
    // (can reach 150Mhz if connect short & high-quality)
    module.spi->regs.gcr.timing_mode_sel = 1; // enable new mode
    module.spi->regs.tcr.sdc1 = 0; // per manual, do not use this part of spectrum
    if (mhz >= 75) {
        module.spi->regs.tcr.sdm = 0; // one cycle delay for highest speeds
        module.spi->regs.tcr.sdc = 1;
    } else if (mhz >= 24) {
        module.spi->regs.tcr.sdm = 0; // half cycle delay at medium speed
        module.spi->regs.tcr.sdc = 0;
     } else {
        module.spi->regs.tcr.sdm = 1; // normal mode, no delay at lower speeds
        module.spi->regs.tcr.sdc = 0;
    }
}

void spid_init(void) {
    // this driver code supports only SPI 1 which is broken out on GPIO header
    module.spi = &module.spi_base[1];

    gpio_set_function(module.clock, module.fn_spi);
    gpio_set_function(module.mosi, module.fn_spi);
    gpio_set_function(module.miso, module.fn_spi);

    ccu_ungate_bus_clock_bits(CCU_SPI_BGR_REG, (1 << 1), (1 << 17)); // SPI 1

    module.spi->regs.tcr.ssowner = 1; // use manual control for chip select
    module.spi->regs.gcr.master_mode_sel = 1;
    module.spi->regs.gcr.en = 1;    // enable
}

static void set_spi_mode(spi_mode_t mode) {
    // set phase and polarity according to mode
    module.spi->regs.tcr.cpol = (mode >> 1) & 1;
    module.spi->regs.tcr.cpha = (mode >> 0) & 1;
}

static void activate_device(spi_device_t *dev) {
    module.spi->regs.gcr.en = 1;    // disable while changing
    set_spi_mode(dev->mode);
    config_clock(dev->rate);
    module.spi->regs.gcr.en = 1;    // re-enable
    gpio_write(dev->chip_select, 0);     // chip select active low
}

static void deactivate_device(spi_device_t *dev) {
    gpio_write(dev->chip_select, 1); // release chip select
}

spi_device_t * spid_new_device(gpio_id_t chip_select, spi_mode_t mode, long rate) {
    if (module.spi == NULL) error("spi_init() has not been called!\n");
    spi_device_t *dev = malloc(sizeof(*dev));
    assert(dev != NULL);
    dev->mode = mode;
    dev->chip_select = chip_select;
    dev->rate = rate;
    dev->generation = -1;
    dev->has_tx = dev->has_rx = false;

    gpio_set_output(dev->chip_select);
    gpio_write(dev->chip_select, 1); // select idle high

    return dev;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static void large_transfer_dma_nb(spi_device_t *dev, const uint8_t *tx, int tx_len, uint8_t *rx, int rx_len, bool serialize) {
    // configure the total number of spi bursts
    module.spi->regs.mbc = serialize ? tx_len + rx_len : MAX(tx_len, rx_len);

    // after we have depleted the data in the tx buffer we send dummy data.
    module.spi->regs.mtc     = tx_len;
    module.spi->regs.bcc.stc = tx_len;

    // The number of bits we write at once to the txd and rxd registers. This
    // doesn't seem to have any performance impact so the only considerations are
    // 1) This should be <= 32 since the txd and rxd registers are only 32 bits
    // 2) This effects the trigger levels. See below.
    const dma_width_t DATA_WIDTH = DMA_BITWIDTH_32;

    // About these trigger levels: The TX and RX FIFOs are each 64 bits. The 
    // purpose of the trigger levels is to ensure that when the DMA writes to or
    // reads from the FIFOs there is enough data to read and enough space to
    // write. If we don't ensure this various wacky things happen. The relevent
    // number is the number of bytes the DMA writes/reads at once. This is equal
    // to the data width (chosen above) times the DMA burst count. In the DMA
    // driver we have the burst count set at 2. 
    // 
    // For the TX trigger level there is an additional complication that the
    // TX FIFO seems to have four spare bytes in it. This makes the general
    // formula for an acceptable tx trigger level:
    //            trig_level = 64 + 4 - data_width * burst_cnt.
    // Right now we set the trigger level to 32 which will work for a wide range
    // of values.
    module.spi->regs.fcr.tx_trig_level = 0x20;
    // The RX trigger level should match the data from one transaction:
    //             trig_level = data_width * burst_cnt.
    // Here I set it to 8 to match the current 32 bit width set above. If you
    // don't set the trig_level exactly then there might be data left in the 
    // FIFO after the transaction is complete. Currently we always clear that
    // out but if it matters it is something to keep in mind.
    module.spi->regs.fcr.rx_trig_level = 0x08;

    // make sure that the DMA is triggered properly
    module.spi->regs.fcr.tx_drq_en = 1;
    module.spi->regs.fcr.rx_drq_en = 1;

    // begin the transfers
    module.spi->regs.isr.tx_complete = 1;
    module.spi->regs.tcr.dhb = serialize; // don't receive during tx period if we are serialized
    module.spi->regs.tcr.start_burst = 1;

    // Dispatch the DMA requests to fill/empty the FIFO buffers
    if (rx_len > 0) {
        dma_endpoint_t from_spi = dma_create_endpoint(DRQ_TYPE_SPI1, &module.spi->regs.rxd.b8);
        dma_endpoint_t to_mem   = dma_create_endpoint(DRQ_TYPE_DRAM, rx);
        dev->trans_id_rx        = dma_transfer(from_spi, to_mem, DATA_WIDTH, rx_len);
        dev->has_rx = true;
    }
    if (tx_len > 0) {
        dma_endpoint_t from_mem = dma_create_endpoint(DRQ_TYPE_DRAM, tx);
        dma_endpoint_t to_spi   = dma_create_endpoint(DRQ_TYPE_SPI1, &module.spi->regs.txd.b8);
        dma_transfer(from_mem, to_spi, DATA_WIDTH, tx_len);
        dev->has_tx = true;
    }
    module.generation++;
    dev->generation = module.generation;

    // remember this device so we can clean it up later
    module.transferring_device = dev;
}

static void cleanup_from_dma(void) {
    // disable the DMA again
    module.spi->regs.fcr.tx_drq_en = 0;
    module.spi->regs.fcr.rx_drq_en = 0;

    // clear the rx fifo in case the dma didn't clear it
    while(module.spi->regs.fsr.rx_fifo_cnt > 0) module.spi->regs.rxd.b8;

    module.transferring_device = NULL;
}

static void small_transfer_manual(const uint8_t *tx, int tx_len, uint8_t *rx, int rx_len, bool serialize) {
    // the max length is the total number of bursts the SPI controller will do
    module.spi->regs.mbc = serialize ? tx_len + rx_len : MAX(tx_len, rx_len);

    // after we have depleted the data in the tx buffer we send dummy data.
    module.spi->regs.mtc     = tx_len;
    module.spi->regs.bcc.stc = tx_len;

    // enqueue the tx data in the tx fifo
    for (int i = 0; i < tx_len; i++) {
        module.spi->regs.txd.b8 = tx[i];
    }

    // begin the transfer
    module.spi->regs.isr.tx_complete = 1;
    module.spi->regs.tcr.dhb = serialize; // don't receive during tx period if we are serialized
    module.spi->regs.tcr.start_burst = 1;

    // busy wait for the transfer to complete.
    while (!module.spi->regs.isr.tx_complete);

    for (int i = 0; i < rx_len; i++) {
        rx[i] = module.spi->regs.rxd.b8; // dequeue rx fifo
    }
}

#define FIFO_MAX 63

void spid_transfer_nb(
    spi_device_t *dev, 
    const uint8_t *tx, int tx_len, 
    uint8_t *rx, int rx_len, 
    bool serialize
) {
    if (module.spi == NULL) error("spi_init() has not been called!\n");
    assert(dev != NULL);

    if (module.transferring_device) {
        if (!spid_transfer_completed(module.transferring_device)) {
            error("attempted new SPI transfer while previous transfer still underway.\n"
                  "hint: use spi_await_bus_free to wait for your previous non-blocking transfers to complete."
            );
        }

        deactivate_device(module.transferring_device);
        cleanup_from_dma();
    }

    activate_device(dev);

    if (tx_len > FIFO_MAX || rx_len > FIFO_MAX) {
        large_transfer_dma_nb(dev, tx, tx_len, rx, rx_len, serialize);
    } else {
        small_transfer_manual(tx, tx_len, rx, rx_len, serialize);
        deactivate_device(dev);
    }
}

bool spid_transfer_completed(spi_device_t *dev) {
    if (module.spi == NULL) error("spi_init() has not been called!\n");

    if (!dev) return true;

    // The driver has moved on to a later transfer so we must be done.
    if (dev->generation < module.generation)
        return true;

    // The transfer is complete if we have both finished sending all the bytes
    // and the DMA has finished copying out all the bytes.
    return (!dev->has_tx || module.spi->regs.isr.tx_complete) &&
           (!dev->has_rx || dma_transfer_completed(dev->trans_id_rx));
}

bool spid_is_bus_free(void) {
    if (module.spi == NULL) error("spi_init() has not been called!\n");

    return spid_transfer_completed(module.transferring_device);
}
