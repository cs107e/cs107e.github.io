#pragma once

#include <stdint.h>

void mcp3008_init(void);

uint16_t mcp3008_read(int channel_num);
