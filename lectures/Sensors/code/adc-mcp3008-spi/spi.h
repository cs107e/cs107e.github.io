#pragma once

/*
 * Author: Yifan Yang <yyang29@stanford.edu>
 *
 * Date: Mar 5, 2024
 */

void spi_init(void);
void spi_transfer(unsigned char *tx, unsigned char *rx, int len);
