#pragma once
#include "mailbox.h"
bool ref_mailbox_request(unsigned int channel, unsigned int addr);
bool ref_mailbox_write(unsigned int channel, unsigned int addr);
unsigned int ref_mailbox_read(unsigned int channel);
