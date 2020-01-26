/*
 * Interface to Raspberry Pi mailbox system, used by libpi to interact
 * with GPU. Note that this interface has been deprecated and so future
 * code should use the property interface instead.
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: 6/20/17
 */

#include "mailbox.h"

#define MAILBOX_BASE     0x2000B880

// This bit is set in status register if no space to write to mailbox queue
#define MAILBOX_FULL   (1<<31)

// This bit is set in status register if nothing to read in mailbox queue
#define MAILBOX_EMPTY  (1<<30)

typedef struct {
    unsigned int read;
    unsigned int padding[3]; // note padding to skip 3 words
    unsigned int peek;
    unsigned int sender;
    unsigned int status;
    unsigned int configuration;
    unsigned int write;
} mailbox_t;


// mailbox is volatile because the GPU can also write to it
static volatile mailbox_t *mailbox = (volatile mailbox_t *)MAILBOX_BASE;


void mailbox_write(unsigned int channel, unsigned int addr)
{
    // mailbox has a maximum of 16 channels
    if (channel >= MAILBOX_MAXCHANNEL) {
        return;
    }

    // addresses must be aligned to 16
    if (addr & 0xF) {
        return;
    }

    while (mailbox->status & MAILBOX_FULL) ; // wait until space to write

    // addr is a multiple of 16, so the low 4 bits are zeros
    // 4-bit channel number is stuffed into those low bits
    mailbox->write = addr + channel;
}

unsigned int mailbox_read(unsigned int channel) 
{
    if (channel >= MAILBOX_MAXCHANNEL) {
        return 1;
    }

    while (1) {
        while (mailbox->status & MAILBOX_EMPTY) ;   // wait until something to read
        unsigned int ra = mailbox->read;
        if ((ra & 0xF) == channel) { // if this message is for requested channel
            return (ra >> 4);   // returned data is in the upper 28 bits
        }
    }
}
