#include "dma.h"
#include "malloc.h"
#include "printf.h"
#include "ccu.h"

#define CCU_BASE 0x2001000UL
#define DMA_BGR_REG (CCU_BASE + 0x70C)

struct DMA_BGR {
    uint32_t DMA_GATING : 1;
    uint32_t UNUSED1 : 15;
    uint32_t DMA_RST : 1;
    uint32_t UNUSED2 : 15;
};

struct DMA_DESCRIPTOR *dma_init(const void *source_addr, volatile void *dest_addr, uint32_t byte_count) {
    // gate the correct clock
    // volatile struct DMA_BGR *dma_bgr = (struct DMA_BGR *)DMA_BGR_REG;
    // dma_bgr->DMA_RST = 1;
    // dma_bgr->DMA_GATING = 1;

    struct DMA_DESCRIPTOR *dma_descriptor = malloc(sizeof(struct DMA_DESCRIPTOR));
    printf("sizeof(struct DMA_DESCRIPTOR): %ld\n", sizeof(struct DMA_DESCRIPTOR));
    dma_descriptor->config.DMA_SRC_DRQ_TYPE = 1;
    dma_descriptor->config.DMA_SRC_BLOCK_SIZE = 0;
    dma_descriptor->config.DMA_SRC_ADDR_MODE = 0;
    dma_descriptor->config.DMA_SRC_DATA_WIDTH = 2;
    dma_descriptor->config.DMA_DEST_DRQ_TYPE = 5;
    dma_descriptor->config.DMA_DEST_BLOCK_SIZE = 0;
    dma_descriptor->config.DMA_DEST_ADDR_MODE = 1;
    dma_descriptor->config.DMA_DEST_DATA_WIDTH = 2;
    dma_descriptor->config.BMODE_SEL = 0;
    printf("before source: %p\n", source_addr);
    printf("before dest:   %p\n", dest_addr);
    dma_descriptor->source_addr = (uint32_t)((uint64_t)source_addr & 0xffffffff);
    dma_descriptor->dest_addr = (uint32_t)((uint64_t)dest_addr & 0xffffffff);
    dma_descriptor->byte_count = byte_count;
    printf("byte count: %d\n", dma_descriptor->byte_count);
    dma_descriptor->parameter.WAIT_CLOCK_CYCLES = 0;
    dma_descriptor->parameter.HIGH2_SRC = (uint32_t)(((uint64_t)source_addr >> 32) & 0x3);
    dma_descriptor->parameter.HIGH2_DEST = (uint32_t)(((uint64_t)dest_addr >> 32) & 0x3);
    dma_descriptor->link.full = 0xFFFFF800; // only one memory address
    printf("after source: %x, %x\n", *(uint32_t *)(&dma_descriptor->source_addr), *(uint32_t *)(&dma_descriptor->dest_addr));
    printf("parameter: %x\n", *(uint32_t *)(&dma_descriptor->parameter));
    printf("link: %x\n", *(uint32_t *)(&dma_descriptor->link));
    /*
    volatile struct HSTIMER_BGR_REG *hstimer_bgr_reg = (struct HSTIMER_BGR_REG *)(CCU_BASE + 0x073C);
    volatile uint32_t *mbus_mat_clk_gating_reg = (uint32_t *)(CCU_BASE + 0x804);
    hstimer_bgr_reg->HSTIMER_RST = 1;
    hstimer_bgr_reg->HSTIMER_GATING = 1;
    *mbus_mat_clk_gating_reg = (*mbus_mat_clk_gating_reg) | 1; // gate clock
    printf("mbus_mat_clk: %p, %x\n", mbus_mat_clk_gating_reg, *mbus_mat_clk_gating_reg);
    */
    volatile struct DMA *dmac = (struct DMA *)DMAC_BASE;
    // dmac->dmac_channel[0].dmac_desc_addr_regn.full = (uint64_t)dma_descriptor;
    dmac->dmac_auto_gate_reg.DMA_MCLK_CIRCUIT = 1; // autogating off 
    printf("dmac->dmac_auto_gate_reg.DMA_MCLK_CIRCUIT: %p, %x\n", &dmac->dmac_auto_gate_reg, *(uint32_t *) &dmac->dmac_auto_gate_reg);
    printf("descriptor: %p, %x\n", dma_descriptor, *(uint32_t *) dma_descriptor);
    /*
    uint32_t low_addr_bytes = (uint64_t)dma_descriptor;
    printf("dma channel 0: %p\n", &dmac->dmac_channel[0]);
    dmac->dmac_channel[0].dmac_desc_addr_regn.parts.DMA_DESC_ADDR = low_addr_bytes; 
    dmac->dmac_channel[0].dmac_desc_addr_regn.parts.DMA_DESC_HIGH_ADDR = (uint64_t)dma_descriptor >> 30;
    printf("high bits: %x\n", dmac->dmac_channel[0].dmac_desc_addr_regn.full & 0x3);
    */
    // dmac->dmac_channel[0].dmac_desc_addr_regn.full = (uint64_t)dma_descriptor;
    dmac->dmac_channel[0].dmac_desc_addr_regn = (uint64_t)dma_descriptor;
    /*
    do{
        printf("dma channel 0 dmac_desc_addr_regn.full: %p, %x\n", &dmac->dmac_channel[0].dmac_desc_addr_regn.full, dmac->dmac_channel[0].dmac_desc_addr_regn.full);
        if(dmac->dmac_channel[0].dmac_desc_addr_regn.parts.DMA_DESC_ADDR == low_addr_bytes) {
    //    if(dmac->dmac_channel[0].dmac_desc_addr_regn.full == (uint64_t)dma_descriptor) {
            break;
        }
    } while(1);
    */

    // dmac->dmac_irq_en_reg0.DMA0_QUEUE_IRQ_EN = 1;
    // dmac->dmac_irq_en_reg0.DMA0_PKG_IRQ_EN = 1;
    /*
    while (1) {
    printf("dmac_cfg_regn: %x\n", *(uint32_t *)(&dmac->dmac_channel[0].dmac_cfg_regn));
    }
    */

    return dma_descriptor;
}

void dma_start() {
    volatile struct DMA *dmac = (struct DMA *)DMAC_BASE;
    dmac->dmac_auto_gate_reg.DMA_MCLK_CIRCUIT = 0; // autogating on 
    printf("dmac->dmac_auto_gate_reg.DMA_MCLK_CIRCUIT: %p, %x\n", &dmac->dmac_auto_gate_reg, *(uint32_t *) &dmac->dmac_auto_gate_reg);
    dmac->dmac_channel[0].dmac_en_regn.DMA_EN = 1;
    printf("dmac_channel[0].dmac_en_regn.DMA_EN: %p, %x\n", &dmac->dmac_channel[0].dmac_en_regn, *(uint32_t *) &dmac->dmac_channel[0].dmac_en_regn);
}

void dma_disable(int channel) {
    volatile struct DMA *dmac = (struct DMA *)DMAC_BASE;
    dmac->dmac_channel[channel].dmac_en_regn.DMA_EN = 0;
}

int dma_complete(int channel) {
    volatile struct DMA *dmac = (struct DMA *)DMAC_BASE;
    uint32_t status = ((dmac->dmac_sta_reg.DMA_STATUS & 0xffff) >> channel) & 1;
    return !status;
}

struct DMA_DESCRIPTOR *dma_mic_init(volatile void *source_addr, void *dest_addr, uint32_t byte_count) {
    // use channel 1 for mic (b/c we might be using channel 0 for audio output)
    struct DMA_DESCRIPTOR *dma_descriptor = malloc(sizeof(struct DMA_DESCRIPTOR));
    printf("sizeof(struct DMA_DESCRIPTOR): %ld\n", sizeof(struct DMA_DESCRIPTOR));
    dma_descriptor->config.DMA_SRC_DRQ_TYPE = 5;
    dma_descriptor->config.DMA_SRC_BLOCK_SIZE = 0;
    dma_descriptor->config.DMA_SRC_ADDR_MODE = 1;
    dma_descriptor->config.DMA_SRC_DATA_WIDTH = 2;
    dma_descriptor->config.DMA_DEST_DRQ_TYPE = 1;
    dma_descriptor->config.DMA_DEST_BLOCK_SIZE = 0;
    dma_descriptor->config.DMA_DEST_ADDR_MODE = 0;
    dma_descriptor->config.DMA_DEST_DATA_WIDTH = 2;
    dma_descriptor->config.BMODE_SEL = 0;
    printf("before source: %p\n", source_addr);
    printf("before dest:   %p\n", dest_addr);
    dma_descriptor->source_addr = (uint32_t)((uint64_t)source_addr & 0xffffffff);
    dma_descriptor->dest_addr = (uint32_t)((uint64_t)dest_addr & 0xffffffff);
    dma_descriptor->byte_count = byte_count;
    printf("byte count: %d\n", dma_descriptor->byte_count);
    dma_descriptor->parameter.WAIT_CLOCK_CYCLES = 0;
    dma_descriptor->parameter.HIGH2_SRC = (uint32_t)(((uint64_t)source_addr >> 32) & 0x3);
    dma_descriptor->parameter.HIGH2_DEST = (uint32_t)(((uint64_t)dest_addr >> 32) & 0x3);
    dma_descriptor->link.full = 0xFFFFF800; // only one memory address
    printf("after source: %x, %x\n", *(uint32_t *)(&dma_descriptor->source_addr), *(uint32_t *)(&dma_descriptor->dest_addr));
    printf("parameter: %x\n", *(uint32_t *)(&dma_descriptor->parameter));
    printf("link: %x\n", *(uint32_t *)(&dma_descriptor->link));
    volatile struct DMA *dmac = (struct DMA *)DMAC_BASE;
    dmac->dmac_auto_gate_reg.DMA_MCLK_CIRCUIT = 1; // autogating off 
    printf("dmac->dmac_auto_gate_reg.DMA_MCLK_CIRCUIT: %p, %x\n", &dmac->dmac_auto_gate_reg, *(uint32_t *) &dmac->dmac_auto_gate_reg);
    printf("descriptor: %p, %x\n", dma_descriptor, *(uint32_t *) dma_descriptor);
    dmac->dmac_channel[1].dmac_desc_addr_regn = (uint64_t)dma_descriptor;

    return dma_descriptor;
}

void dma_mic_start() {
    volatile struct DMA *dmac = (struct DMA *)DMAC_BASE;
    dmac->dmac_auto_gate_reg.DMA_MCLK_CIRCUIT = 0; // autogating on 
    printf("dmac->dmac_auto_gate_reg.DMA_MCLK_CIRCUIT: %p, %x\n", &dmac->dmac_auto_gate_reg, *(uint32_t *) &dmac->dmac_auto_gate_reg);
    dmac->dmac_channel[1].dmac_en_regn.DMA_EN = 1;
    printf("dmac_channel[1].dmac_en_regn.DMA_EN: %p, %x\n", &dmac->dmac_channel[1].dmac_en_regn, *(uint32_t *) &dmac->dmac_channel[1].dmac_en_regn);
    printf("sizeof(struct DMAC_CHANNEL): %ld\n", sizeof(struct DMAC_CHANNEL));
}

