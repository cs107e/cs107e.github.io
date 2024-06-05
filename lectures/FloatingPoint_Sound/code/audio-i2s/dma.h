#ifndef DMA_H
#define DMA_H
#include "stdint.h"

#define DMAC_BASE 0x03002000UL

struct DMAC_IRQ_EN_REG0 {
    uint32_t DMA0_HLAF_IRQ_EN : 1;
    uint32_t DMA0_PKG_IRQ_EN : 1;
    uint32_t DMA0_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED1 : 1;
    uint32_t DMA1_HLAF_IRQ_EN : 1;
    uint32_t DMA1_PKG_IRQ_EN : 1;
    uint32_t DMA1_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED2 : 1;
    uint32_t DMA2_HLAF_IRQ_EN : 1;
    uint32_t DMA2_PKG_IRQ_EN : 1;
    uint32_t DMA2_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED3 : 1;
    uint32_t DMA3_HLAF_IRQ_EN : 1;
    uint32_t DMA3_PKG_IRQ_EN : 1;
    uint32_t DMA3_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED4 : 1;
    uint32_t DMA4_HLAF_IRQ_EN : 1;
    uint32_t DMA4_PKG_IRQ_EN : 1;
    uint32_t DMA4_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED5 : 1;
    uint32_t DMA5_HLAF_IRQ_EN : 1;
    uint32_t DMA5_PKG_IRQ_EN : 1;
    uint32_t DMA5_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED6 : 1;
    uint32_t DMA6_HLAF_IRQ_EN : 1;
    uint32_t DMA6_PKG_IRQ_EN : 1;
    uint32_t DMA6_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED7 : 1;
    uint32_t DMA7_HLAF_IRQ_EN : 1;
    uint32_t DMA7_PKG_IRQ_EN : 1;
    uint32_t DMA7_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED8 : 1;
};

struct DMAC_IRQ_EN_REG1 {
    uint32_t DMA8_HLAF_IRQ_EN : 1;
    uint32_t DMA8_PKG_IRQ_EN : 1;
    uint32_t DMA8_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED1 : 1;
    uint32_t DMA9_HLAF_IRQ_EN : 1;
    uint32_t DMA9_PKG_IRQ_EN : 1;
    uint32_t DMA9_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED2 : 1;
    uint32_t DMA10_HLAF_IRQ_EN : 1;
    uint32_t DMA10_PKG_IRQ_EN : 1;
    uint32_t DMA10_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED3 : 1;
    uint32_t DMA11_HLAF_IRQ_EN : 1;
    uint32_t DMA11_PKG_IRQ_EN : 1;
    uint32_t DMA11_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED4 : 1;
    uint32_t DMA12_HLAF_IRQ_EN : 1;
    uint32_t DMA12_PKG_IRQ_EN : 1;
    uint32_t DMA12_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED5 : 1;
    uint32_t DMA13_HLAF_IRQ_EN : 1;
    uint32_t DMA13_PKG_IRQ_EN : 1;
    uint32_t DMA13_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED6 : 1;
    uint32_t DMA14_HLAF_IRQ_EN : 1;
    uint32_t DMA14_PKG_IRQ_EN : 1;
    uint32_t DMA14_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED7 : 1;
    uint32_t DMA15_HLAF_IRQ_EN : 1;
    uint32_t DMA15_PKG_IRQ_EN : 1;
    uint32_t DMA15_QUEUE_IRQ_EN : 1;
    uint32_t UNUSED8 : 1;
};

struct DMAC_IRQ_PEND_REG0 {
    uint32_t DMA0_HLAF_IRQ_PEND : 1;
    uint32_t DMA0_PKG_IRQ_PEND : 1;
    uint32_t DMA0_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED1 : 1;
    uint32_t DMA1_HLAF_IRQ_PEND : 1;
    uint32_t DMA1_PKG_IRQ_PEND : 1;
    uint32_t DMA1_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED2 : 1;
    uint32_t DMA2_HLAF_IRQ_PEND : 1;
    uint32_t DMA2_PKG_IRQ_PEND : 1;
    uint32_t DMA2_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED3 : 1;
    uint32_t DMA3_HLAF_IRQ_PEND : 1;
    uint32_t DMA3_PKG_IRQ_PEND : 1;
    uint32_t DMA3_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED4 : 1;
    uint32_t DMA4_HLAF_IRQ_PEND : 1;
    uint32_t DMA4_PKG_IRQ_PEND : 1;
    uint32_t DMA4_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED5 : 1;
    uint32_t DMA5_HLAF_IRQ_PEND : 1;
    uint32_t DMA5_PKG_IRQ_PEND : 1;
    uint32_t DMA5_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED6 : 1;
    uint32_t DMA6_HLAF_IRQ_PEND : 1;
    uint32_t DMA6_PKG_IRQ_PEND : 1;
    uint32_t DMA6_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED7 : 1;
    uint32_t DMA7_HLAF_IRQ_PEND : 1;
    uint32_t DMA7_PKG_IRQ_PEND : 1;
    uint32_t DMA7_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED8 : 1;
};

struct DMAC_IRQ_PEND_REG1 {
    uint32_t DMA8_HALF_IRQ_PEND : 1;
    uint32_t DMA8_PKG_IRQ_PEND : 1;
    uint32_t DMA8_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED1 : 1;
    uint32_t DMA9_HLAF_IRQ_PEND : 1;
    uint32_t DMA9_PKG_IRQ_PEND : 1;
    uint32_t DMA9_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED2 : 1;
    uint32_t DMA10_HLAF_IRQ_PEND : 1;
    uint32_t DMA10_PKG_IRQ_PEND : 1;
    uint32_t DMA10_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED3 : 1;
    uint32_t DMA11_HLAF_IRQ_PEND : 1;
    uint32_t DMA11_PKG_IRQ_PEND : 1;
    uint32_t DMA11_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED4 : 1;
    uint32_t DMA12_HLAF_IRQ_PEND : 1;
    uint32_t DMA12_PKG_IRQ_PEND : 1;
    uint32_t DMA12_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED5 : 1;
    uint32_t DMA13_HLAF_IRQ_PEND : 1;
    uint32_t DMA13_PKG_IRQ_PEND : 1;
    uint32_t DMA13_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED6 : 1;
    uint32_t DMA14_HLAF_IRQ_PEND : 1;
    uint32_t DMA14_PKG_IRQ_PEND : 1;
    uint32_t DMA14_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED7 : 1;
    uint32_t DMA15_HLAF_IRQ_PEND : 1;
    uint32_t DMA15_PKG_IRQ_PEND : 1;
    uint32_t DMA15_QUEUE_IRQ_PEND : 1;
    uint32_t UNUSED8 : 1;
};

struct DMAC_AUTO_GATE_REG {
    uint32_t DMA_CHAN_CIRCUIT : 1;
    uint32_t DMA_COMMON_CIRCUIT : 1;
    uint32_t DMA_MCLK_CIRCUIT : 1;
    uint32_t UNUSED : 29;
};

struct DMAC_STA_REG {
    uint32_t DMA_STATUS : 16;
    uint32_t UNUSED : 15;
    uint32_t MBUS_FIFO_STATUS : 1;
};

struct DMAC_EN_REGN {
    uint32_t DMA_EN : 1;
    uint32_t UNUSED : 31;
};

struct DMAC_PAU_REGN {
    uint32_t DMA_PAUSE : 1;
    uint32_t UNUSED : 31;
};

/*
// The manual is INCORRECT about this crazy behavior
typedef union {
    struct DMAC_DESC_ADDR_REGN {
        uint32_t DMA_DESC_HIGH_ADDR : 2;
        uint32_t DMA_DESC_ADDR : 30;
    } parts;
    uint32_t full;
} DMAC_DESC_ADDR_REGN;
*/

struct DMAC_CFG_REGN {
    uint32_t DMA_SRC_DRQ_TYPE : 6;
    uint32_t DMA_SRC_BLOCK_SIZE : 2;
    uint32_t DMA_SRC_ADDR_MODE : 1;
    uint32_t DMA_SRC_DATA_WIDTH : 2;
    uint32_t UNUSED1 : 5;
    uint32_t DMA_DEST_DRQ_TYPE : 6;
    uint32_t DMA_DEST_BLOCK_SIZE : 2;
    uint32_t DMA_DEST_ADDR_MODE : 1;
    uint32_t DMA_DEST_DATA_WIDTH : 2;
    uint32_t UNUSED2 : 3;
    uint32_t BMODE_SEL : 1;
    uint32_t UNUSED3 : 1;
};

struct DMAC_BCNT_LEFT_REGN {
    uint32_t DMA_BCNT_LEFT : 25;
    uint32_t UNUSED1 : 7;
};

struct DMAC_PARA_REGN {
    uint32_t WAIT_CYC : 8;
    uint32_t UNUSED1 : 24;
};

struct DMAC_MODE_REGN {
    uint32_t UNUSED1 : 2;
    uint32_t DMA_SRC_MODE : 1;
    uint32_t DMA_DST_MODE : 1;
    uint32_t UNUSED2 : 28;
};

struct DMAC_CHANNEL {
    struct DMAC_EN_REGN dmac_en_regn;
    struct DMAC_PAU_REGN dmac_pau_regn;
    // DMAC_DESC_ADDR_REGN dmac_desc_addr_regn;
    uint32_t dmac_desc_addr_regn;
    struct DMAC_CFG_REGN dmac_cfg_regn;
    uint32_t DMAC_CUR_SRC_REGN;
    uint32_t DMAC_CUR_DEST_REGN;
    struct DMAC_BCNT_LEFT_REGN dmac_bcnt_left_regn;
    struct DMAC_PARA_REGN dmac_para_regn;
    uint32_t UNUSED1;
    uint32_t UNUSED2;
    uint32_t UNUSED3;
    struct DMAC_MODE_REGN dmac_mode_regn;
    uint32_t DMAC_FDESC_ADDR_REGN;
    uint32_t DMAC_PKG_NUM_REGN;
    uint32_t PADDING[2];
};

struct DMA {
    struct DMAC_IRQ_EN_REG0 dmac_irq_en_reg0; // offset 0x0
    struct DMAC_IRQ_EN_REG1 dmac_irq_en_reg1; // offset 0x4
    uint32_t padding1[2]; // offset 0x8
    struct DMAC_IRQ_PEND_REG0 dmac_irq_pend_reg0; // offset 0x10
    struct DMAC_IRQ_PEND_REG1 dmac_irq_pend_reg1; // offset 0x14
    uint32_t padding2[4]; // offset 0x18
    struct DMAC_AUTO_GATE_REG dmac_auto_gate_reg; // offset 0x28
    uint32_t padding3[1]; // offset 0x2c
    struct DMAC_STA_REG dmac_sta_reg; // offset 0x30
    uint32_t padding4[51]; // offset 0x34
    struct DMAC_CHANNEL dmac_channel[16]; 
};

struct DMA_PARAMETER {
    uint32_t WAIT_CLOCK_CYCLES : 8;
    uint32_t UNUSED1 : 8;
    uint32_t HIGH2_SRC : 2;
    uint32_t HIGH2_DEST : 2;
    uint32_t UNUSED2 : 12;
};

typedef union {
    struct DMA_LINK { 
        uint32_t NEXT_GROUP_DESCRIPTOR_HIGH2 : 2;
        uint32_t NEXT_GROUP_DESCRIPTOR_LOW30 : 30; 
    } bits;
    uint32_t full;
} DMA_LINK;

struct DMA_DESCRIPTOR {
    struct DMAC_CFG_REGN config;
    uint32_t source_addr;
    uint32_t dest_addr;
    uint32_t byte_count;
    struct DMA_PARAMETER parameter;
    DMA_LINK link;
};

struct DMA_DESCRIPTOR *dma_init(const void *source_addr, volatile void *dest_addr, uint32_t byte_count);

struct DMA_DESCRIPTOR *dma_mic_init(volatile void *source_addr, void *dest_addr, uint32_t byte_count);

struct HSTIMER_BGR_REG {
    uint32_t HSTIMER_GATING : 1;
    uint32_t UNUSED1 : 15;
    uint32_t HSTIMER_RST : 1;
    uint32_t UNUSED2 : 15;
};

void dma_start();
void dma_disable(int channel);
void dma_mic_start();
int dma_complete(int channel);

#endif
