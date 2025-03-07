/*
 * DMA controller
 *
 * Author: Chris Gregg <cgregg@stanford.edu>
 * Author: Daniel James <drjames@stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "dma.h"
#include "ccu.h"

#define LINK_END_SENTINEL 0xFFFFF800

typedef enum {
    BLOCK_1 = 0,
    BLOCK_4,
    BLOCK_8,
    BLOCK_16,
} dma_block_size_t;

typedef struct {
    struct {
        uint32_t src_drq_type   : 6;
        uint32_t src_blk_size   : 2;
        uint32_t src_addr_mode  : 1;
        uint32_t src_data_width : 2;
        uint32_t                : 5;
        uint32_t dst_drq_type   : 6;
        uint32_t dst_blk_size   : 2;
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

typedef struct {
    uint32_t irq_enable[2];
    uint32_t reserved0[2];
    uint32_t irq_pending_status[2];
    uint32_t reserved1[4];

    struct {
        uint32_t chan_circuit   : 1;
        uint32_t common_circuit : 1;
        uint32_t mclk_circuit   : 1; // Important to set this on init.
        uint32_t                :29;
    } auto_gating;
    uint32_t reserved2;

    struct {
        uint32_t dma_status       :16;
        uint32_t                  :15;
        uint32_t mbus_fifo_status : 1;
    } status;
    uint8_t reserved3[204];

    struct {
        uint32_t enable;
        uint32_t pause;
        uint32_t desc_addr;
        uint32_t config;    // read only (will be copied from descriptor)
        uint32_t src_addr;  // read only
        uint32_t dst_addr;  // read only
        uint32_t byte_cnt;  // read only
        uint32_t params;    // read only
        uint32_t reserved1[2];
        struct {
            uint32_t          : 2;
            uint32_t src_mode : 1;
            uint32_t dst_mode : 1;
            uint32_t          :28;
        } mode;
        uint32_t former_desc_addr; // read only
        uint32_t pkg_cnt;          // read only
        uint32_t reserved2[3];
    } channels[16];
} dmac_t;

#define DMAC_BASE ((dmac_t *)0x03002000)
_Static_assert(sizeof(dma_desc_t)  == 6*sizeof(uint32_t), "dma descriptor must be size 6 32-bit words");
_Static_assert(&(DMAC_BASE->status) == (void *)0x03002030, "dma status reg must be at 0x03002030");
_Static_assert(&(DMAC_BASE->channels[1].enable) == (void *)0x03002140, "dma channel 1 must start at 0x03002140");

static struct {
    volatile dmac_t * const dmac;
    dma_desc_t descriptors[16];
} module = {
    .dmac = DMAC_BASE,
};

void dma_init(void) {
    ccu_ungate_bus_clock(CCU_DMA_BGR_REG); // pg. 225
    module.dmac->auto_gating.mclk_circuit = 1; // must init (pg 243), disable autogating
}

static dma_channel_id_t find_open_channel(void) {
    for (int i = 0; i < 16; i++) {
        if (module.dmac->channels[i].enable == 0) {
            return i;
        }
    }
    return DMA_CHANNEL_NONE;
}

dma_channel_id_t dma_new_channel(dma_endpoint_t src, dma_endpoint_t dst, int nbytes) {
    dma_channel_id_t ch = find_open_channel();
    if (ch == DMA_CHANNEL_NONE) return ch;
    dma_desc_t *desc = &module.descriptors[ch];
    desc->config.src_drq_type = src.type;
    desc->config.src_blk_size = BLOCK_1;
    desc->config.src_addr_mode = src.mode;
    desc->config.src_data_width = src.width;
    desc->config.dst_drq_type = dst.type;
    desc->config.dst_blk_size = BLOCK_1;
    desc->config.dst_addr_mode = dst.mode;
    desc->config.dst_data_width = dst.width;
    desc->config.bmode_sel = 0; // normal
    desc->src = src.addr; // assignment truncates to lower 32-bits
    desc->dst = dst.addr;
    // extra 2 high bits of 34-bit address stored here, ignore since our addresses are 32-bit
    desc->param.src_high_bits = desc->param.dst_high_bits = 0;
    desc->byte_cnt = nbytes;
    desc->param.wait_clk_cycles = 0;
    desc->link = LINK_END_SENTINEL; // no linked packets
    module.dmac->channels[ch].desc_addr = (uintptr_t)desc;
    return ch;
}

void dma_begin_transfer(dma_channel_id_t ch) {
    module.dmac->channels[ch].enable = 1;
}

bool dma_end_transfer(dma_channel_id_t ch) {
    if ((module.dmac->status.dma_status & (1 << ch)) == 0) {
        module.dmac->channels[ch].enable = 0;
        return true;
    } else {
        return false;
    }
}
