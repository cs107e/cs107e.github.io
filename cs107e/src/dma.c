/*
 * File: dma.c
 *
 * DMA controller
 *
 * Author: Daniel James <drjames@stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 *
 * Last updated: Curis summer 2025
 */

#include "dma.h"
#include "assert.h"
#include "ccu.h"

typedef struct {
    struct {
        uint32_t src_drq_type   : 6;
        uint32_t src_burst_cnt  : 2; // note: erroneously called block size in the manual
        uint32_t src_addr_mode  : 1;
        uint32_t src_data_width : 2;
        uint32_t                : 5;
        uint32_t dst_drq_type   : 6;
        uint32_t dst_burst_cnt  : 2; // note: erroneously called block size in the manual
        uint32_t dst_addr_mode  : 1;
        uint32_t dst_data_width : 2;
        uint32_t                : 3;
        uint32_t bmode_sel      : 1;
        uint32_t                : 1;
    } config;
    uint32_t src;
    uint32_t dst;
    uint32_t byte_cnt;
    struct {
        uint32_t wait_clk_cycles : 8;
        uint32_t                 : 8;
        uint32_t src_high_bits   : 2;
        uint32_t dst_high_bits   : 2;
        uint32_t                 :12;
    } param;
    uint32_t link;
} dma_desc_t;

#define LINK_END_SENTINEL 0xFFFFF800

typedef enum {
    DMA_CHANNEL_NONE = -1,
    DMA_N_CHANNELS = 16,
} dma_channel_id_t;

typedef enum {
    // The manual claims that bit 0 corresponds to burst count of 1 but some of
    // our observations seems to suggest it may actually be 2 (but honestly, we are
    // not really sure).  When in normal (e.g. not bmode), the burst
    // setting seems to be ignored, so may not matter what it is set to.
    DMA_BURST_CNT_2 = 0,
    DMA_BURST_CNT_4,
    DMA_BURST_CNT_8,
    DMA_BURST_CNT_16,
} dma_burst_cnt_t;

typedef enum {
    DMA_NORMAL_MODE = 0,
    DMA_BMODE,
} dma_bmode_t;

typedef struct {
    uint32_t irq_enable[2];
    uint32_t _resa[2];
    uint32_t irq_pending_status[2];
    uint32_t _resb[4];
    struct {
        uint32_t chan_circuit   : 1;
        uint32_t common_circuit : 1;
        uint32_t mclk_circuit   : 1; // Important to set this on init
        uint32_t                :29;
    } auto_gating;
    uint32_t _resc;
    const struct {
        uint32_t dma_status     :16;
        uint32_t                :15;
        uint32_t mbus_status    : 1;
    } status;
    uint8_t _resd[204];
    struct {
        uint32_t enable;
        uint32_t pause;
        uint32_t desc_addr;
        const uint32_t config;    // these read-only values are copied from descriptor
        const uint32_t src_addr;
        const uint32_t dst_addr;
        const uint32_t byte_cnt_left;
        const uint32_t params;
        const uint32_t _rese[2];
        uint32_t mode;          // we are using default waiting mode (do we need to support handshake?)
        const uint32_t former_desc_addr;
        const uint32_t pkg_cnt;
        const uint32_t _resf[3];
    } channels[DMA_N_CHANNELS];
} dmac_t;

#define DMAC_BASE ((dmac_t *)0x03002000)
_Static_assert(sizeof(dma_desc_t)  == 6*sizeof(uint32_t), "dma descriptor must be size 6 32-bit words");
_Static_assert(&(DMAC_BASE->status)      == (void *)0x03002030, "dma status reg must be at 0x03002030");
_Static_assert(&(DMAC_BASE->channels[1]) == (void *)0x03002140, "dma channel 1 must start at 0x03002140");

static struct {
    volatile dmac_t * dmac; // non-NULL after init()
    dma_desc_t descriptors[DMA_N_CHANNELS];

    // each time a DMA transfer completes we increment the generation number.
    // this lets us tell if a transfer is complete even if we later reuse the
    // channel for a new transfer.
    int generation[DMA_N_CHANNELS];
} module;

static dma_addr_mode_t mode_for_drq(dma_drq_type_t drq) {
    switch (drq) {
        case DRQ_TYPE_TWI0:
        case DRQ_TYPE_I2S2:
        case DRQ_TYPE_SPI1:
            return DMA_ADDR_MODE_IO;
        case DRQ_TYPE_DRAM:
            return DMA_ADDR_MODE_LINEAR;
    }
    error("Invalid DRQ type");
}

// future TODO: expose link structure for endpoint and/or allow circular link?
dma_endpoint_t dma_create_endpoint(dma_drq_type_t type, const volatile void *addr) {
    dma_addr_mode_t mode = mode_for_drq(type);
    dma_endpoint_t end = { .type = type, .mode = mode, .addr = (uintptr_t)addr };
    return end;
}

void dma_init(void) {
    // The address of the descriptors cannot be more than 2^34 since we only
    // have 32 bits + 2 zero bits to store the address.
    assert((uintptr_t)(&module.descriptors[DMA_N_CHANNELS - 1]) <= ((uintptr_t)1 << 34));

    // Enable DMA gate on MBUS clock. It is already ungated if program started
    // from FEL, but not if proram started from SD card, so we do it here to be sure.
    // It is safe to ungate again if already ungated.
    ccu_ungate_mbus_bits(0b1);

    // Enable the DMA clock. See manual page 225.
    ccu_ungate_bus_clock(CCU_DMA_BGR_REG);

    module.dmac = DMAC_BASE;
    // Disable auto-gatin, but perhaps we should not? See manual page 243.
    module.dmac->auto_gating.mclk_circuit = 1;
}

static bool channel_is_idle(dma_channel_id_t ch) {
    return (module.dmac->status.dma_status & (1 << ch)) == 0;
}

static dma_channel_id_t find_open_channel(void) {
    for (int i = 0; i < DMA_N_CHANNELS; i++) {
        if (channel_is_idle(i)) return i;
    }
    return DMA_CHANNEL_NONE;
}

static uint32_t swizzle_desc_ptr(dma_desc_t *desc) {
    uintptr_t p = (uintptr_t)desc;
    uint32_t lo = p & ~0b11;
    uint32_t hi = (p >> 32) & 0b11;
    return lo | hi;
}

dma_transfer_id_t dma_transfer(dma_endpoint_t src, dma_endpoint_t dst, dma_width_t w, int n) {
    if (!module.dmac) error("dma_init() has not been called!\n");
    assert(w == DMA_BITWIDTH_8 || w == DMA_BITWIDTH_16 || w == DMA_BITWIDTH_32 || w == DMA_BITWIDTH_64);

    assert(n > 0);

    dma_channel_id_t ch = find_open_channel();
    assert(ch != DMA_CHANNEL_NONE);

    // increment generation number now so if someone who used this channel
    // for a previous transfer checks back they will know that it is complete.
    module.generation[ch]++;

    dma_desc_t *desc = &module.descriptors[ch];

    // about burst count: the manual refers to burst count as block size and 
    // claims that it is refers to number of bits. Observed behavior and
    // read linux driver code suggests instead that burst val is the number of
    // transfers done in a single burst when interacting with an IO device.
    // (burst val seems to be ignored/no change when not in bmode?)
    // The unit for burst is not in bits/bytes, appears to be count of data
    // transfers moved across the bus in one burst. (i.e. number of transfers
    // per memory bus arbitration)
    // The performance benefit to increasing burst count seems to only
    // apply if in bmode and IO device. We are currently not using bmode
    // due to its buggy alignment behavior and use default value for burst cnt.
    desc->config.src_burst_cnt = DMA_BURST_CNT_2;

    desc->config.src_data_width = w;
    desc->config.src_drq_type = src.type;
    desc->config.src_addr_mode = src.mode;

    desc->config.dst_burst_cnt = DMA_BURST_CNT_2;

    desc->config.dst_data_width = w;
    desc->config.dst_drq_type = dst.type;
    desc->config.dst_addr_mode = dst.mode;

    // A note about BMODE. We currently do not fully understand what "bmode"
    // (theorized to be burst mode) does. It seems to have some relationship
    // to data-width and block-size (probably more accurately called burst
    // count). One thing we do know is that it is only allowed to be enabled
    // when one endpoint is an IO device and other is memory.  Bmode
    // behaves incorrectly when attempting to auto-align the memory address.
    // Although forcing memory pointers to align on 32-byte boundary seems to
    // avoid the alignment bugs of bmode, it isn't clear that we need to use
    // bmode at all, and can just use normal mode in all cases (which seems to
    // work without issues). The linux driver also does not use bmode.
    desc->config.bmode_sel = DMA_NORMAL_MODE;

    assert(src.addr < ((uintptr_t)1 << 32));
    desc->src = src.addr; // assignment truncates to lower 32-bits
    desc->param.src_high_bits = 0;

    assert(dst.addr < ((uintptr_t)1 << 32));
    desc->dst = dst.addr; // assignment truncates to lower 32-bits
    desc->param.dst_high_bits = 0;

    desc->byte_cnt = n;
    desc->link = LINK_END_SENTINEL; // no linked packets

    // Each wait clock cycle here adds 31.38 ps of delay. This triggers in
    // between every DMA transmission so to find the number of times it triggers
    // you have to divide your data size by block size * data width. This also
    // only triggers when there is an IO device involved. So memory to memory
    // transmissions are (as usual) unaffected. Also important to remember that
    // if something else is limiting your speed (e.g. the SPI clock) you may
    // not observe any difference from changing this.
    desc->param.wait_clk_cycles = 0;
    
    module.dmac->channels[ch].desc_addr = swizzle_desc_ptr(desc);

    // Start the transaction
    module.dmac->channels[ch].enable = 1;

    return (dma_transfer_id_t) { 
        .generation = module.generation[ch],
        .channel = ch,
    };
}

bool dma_transfer_completed(dma_transfer_id_t id) {
    if (!module.dmac) error("dma_init() has not been called!\n");
    assert(id.channel >= 0 && id.channel <= DMA_N_CHANNELS);

    return id.generation < module.generation[id.channel] ||
        channel_is_idle(id.channel);
}
