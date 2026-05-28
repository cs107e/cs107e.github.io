#pragma once
/*
 * File: dma.h
 *
 * Description: Functions to use DMA transfer to copy data between memory and peripherals.
 *
 * Author: Daniel James <drjames@stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * Type: `dma_width_t`
 *
 * Enumerated type to specify width of the data unit moving across bux.
 */
typedef enum {
    DMA_BITWIDTH_8 = 0,
    DMA_BITWIDTH_16,
    DMA_BITWIDTH_32,
    DMA_BITWIDTH_64,
} dma_width_t;

/*
 * Type: `dma_addr_mode_t`
 *
 * Enumerated type to specify whether address increases after each data unit
 * or is fixed (e.g. hardware fifo address).
 */
typedef enum {
    DMA_ADDR_MODE_LINEAR = 0,   // address increases linearly (memory)
    DMA_ADDR_MODE_IO = 1,       // address fixed (fifo)
} dma_addr_mode_t;

/*
 * Type: `dma_drq_type_t`
 *
 * Enumerated type to specify port type of the endpoint.
 */
typedef enum {
    DRQ_TYPE_DRAM = 1,  // values from table pg 217-8
    DRQ_TYPE_I2S2 = 5,
    DRQ_TYPE_SPI1 = 23,
    DRQ_TYPE_TWI0 = 43,
} dma_drq_type_t;

/*
 * Type: `dma_endpoint_t`
 *
 * Struct containing endpoint addr, type, and mode.
 */
typedef struct {
    uintptr_t addr;
    dma_drq_type_t type;
    dma_addr_mode_t mode;
} dma_endpoint_t;

/*
 * Type: `dma_transfer_id_t`
 *
 * Struct containing info about an in-progress transfer.
 */
typedef struct {
    int channel;
    int generation;
} dma_transfer_id_t;


/*
 * `dma_init` : Required initialization for DMA module
 *
 * Initializes the DMA module, start all clocks.
 * Call `dma_init` once
 */
void dma_init(void);

/*
 * `dma_create_endpoint` : fill in endpoint struct
 *
 * Fill in endpoint struct with the desired type and addr. The endpoint mode
 * is set automatically depending on the type (linear for memory, I/O for peripherals)
 */
dma_endpoint_t dma_create_endpoint(dma_drq_type_t type, const volatile void *addr);

/*
 * `dma_transfer` : Start a new dma transfer
 *
 * Initiates a transfer of `nbytes` from `src` endpoint to `dst` endpoint.
 * The `datawidth` parameter specifies the number of bytes that can be moved across
 * the bus in one data unit. The function starts the transfer and returns immediately,
 * the transfer will progress asynchronously. The return value is a `dma_transfer_id_t`
 * that can be used to later check on the status of the transfer .
 */
dma_transfer_id_t dma_transfer(dma_endpoint_t src, dma_endpoint_t dst, dma_width_t datawidth, int nbytes);

/*
 * `dma_transfer_completed` : Check if transfer has finished
 *
 * Call this function to check on the status of a transaction previously initiated by
 * `dma_transfer`. The function immediately returns either `true` (this transfer has finished,
 * all data has been transferred) or `false` (transfer is still in process of transferring data).
 */
bool dma_transfer_completed(dma_transfer_id_t id);
