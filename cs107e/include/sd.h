#ifndef SD_H
#define SD_H

#include <stdbool.h>

/*
 * Block device driver for Raspberry Pi SD host controller
 * (eMMC). First and lowest level of abstraction.
 *
 * Author: Omar Rizwan <osnr@stanford.edu>
 * Date: Apr 30, 2016
 */

#define SD_SECTOR_SIZE 512

// Return true if successful, false otherwise
bool sd_init(void);

/* addr is a sector number, so 1/512 of the byte offset. */
void sd_sector_read(void *buf, unsigned addr);
void sd_sector_write(const void *buf, unsigned addr);

#endif /* SD_H */
