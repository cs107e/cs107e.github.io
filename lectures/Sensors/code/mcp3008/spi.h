#pragma once

void spi_init(void);
void spi_transfer(unsigned char *in, unsigned char *out, int len);
