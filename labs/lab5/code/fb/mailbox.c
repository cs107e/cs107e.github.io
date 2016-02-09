#include "bcm.h"
#include "memory.h"
#include "mailbox.h"

#define MAILBOX_BASE   0x2000B880

#define MAILBOX_FULL   (1<<31)
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

void mailbox_write ( unsigned channel, unsigned addr )
{
  // mailbox has a maximum of 16 channels
  if ( channel >= MAILBOX_MAXCHANNEL ) return;

  // addr must be a multiple of 16
  if ( addr & 0xF ) return;

  volatile mailbox_t *mailbox = (volatile mailbox_t *)MAILBOX_BASE;

  // wait until mailbox is not full ...
  while (mailbox->status & MAILBOX_FULL) ;

  // mail the addr to the channel
  // why can we add the channel to the addr?
  mailbox->write = addr + channel;
}

unsigned mailbox_read ( unsigned channel )
{
  if ( channel >= MAILBOX_MAXCHANNEL ) return 1;

  volatile mailbox_t *mailbox = (volatile mailbox_t *)MAILBOX_BASE;

  while (1)
  {
    // wait until mailbox is not empty ...
    while (mailbox->status & MAILBOX_EMPTY) ;
    // read the address and channel
    unsigned ra = mailbox->read;
    if ((ra & 0xF) == channel)
      // return if we get a message on the channel we want
      return (ra >> 4);
  }
  return 0;
}

