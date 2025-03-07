#pragma once

/*
 * DMA controller
 */

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    DMA_CHANNEL_0 = 0,
    DMA_CHANNEL_1,
    DMA_CHANNEL_2,
    DMA_CHANNEL_3,
    DMA_CHANNEL_4,
    DMA_CHANNEL_5,
    DMA_CHANNEL_6,
    DMA_CHANNEL_7,
    DMA_CHANNEL_8,
    DMA_CHANNEL_9,
    DMA_CHANNEL_10,
    DMA_CHANNEL_11,
    DMA_CHANNEL_12,
    DMA_CHANNEL_13,
    DMA_CHANNEL_14,
    DMA_CHANNEL_15,
    DMA_CHANNEL_NONE = -1,
} dma_channel_id_t;

typedef enum {
    WIDTH_8 = 0, // in bits
    WIDTH_16,
    WIDTH_32,
    WIDTH_64,
} dma_width_t;

typedef enum {
    ADDR_MODE_LINEAR = 0,   // address increases (memory)
    ADDR_MODE_IO = 1,       // address fixed (fifo)
} dma_addr_mode_t;

typedef enum {
    DRQ_TYPE_DRAM = 1,  // values from table pg 217-8
    DRQ_TYPE_I2S2 = 5,
} dma_drq_type_t;

typedef struct  {
    uintptr_t addr;
    dma_drq_type_t type;
    dma_addr_mode_t mode;
    dma_width_t width;
} dma_endpoint_t;

void dma_init(void);

dma_channel_id_t dma_new_channel(dma_endpoint_t src, dma_endpoint_t dst, int nbytes);
void dma_begin_transfer(dma_channel_id_t ch);
bool dma_end_transfer(dma_channel_id_t ch);
