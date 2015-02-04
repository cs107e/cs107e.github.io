/* 
 * Rewritten version of Welch's bootloader.
 * Copyright (c) 2012 David Welch dwelch@dwelch.com
 *
 * The raspberry pi firmware at the time this was written defaults
 * loading at address 0x8000.  Although this bootloader could easily
 * load at 0x0000, it loads at 0x8000 so that the same binaries built
 * for the SD card work with this bootloader.  Change the ARMBASE
 * below to use a different location.
 */
#include "rpi.h"

#define ARMBASE 0x8000

enum { 
       SOH = 0x01,   // Start Of Header
       ACK = 0x06,   // Acknowledge (positive)
       NAK = 0x15,   // Acknowledge (negative)
       EOT = 0x04,   // End of transmission 
       PACKET_SIZE = 132
};

struct serial_channel {
    unsigned addr,
    	     block,
    	     state, // Stores which byte in block we're on
	     crc,
    	     rx;
};

static struct serial_channel 
serial_start() {
  	struct serial_channel s;
	s.block = 1;
    	s.addr = ARMBASE;
    	s.state = 0;
    	s.crc = 0;
    	s.rx = timer_tick();
	return s;
}
static unsigned char 
serial_recv(struct serial_channel *s) {
	// not ready, just keep going.
	while(((uart_lcr() & 0x01) == 0)) {
		// nak every 4 sec
        	if ((timer_tick() - s->rx) >= 4000000) {
            		uart_send(NAK);
            		s->rx += 4000000;
        	}
	}
        unsigned char uc = uart_recv();
        s->rx = timer_tick();
	return uc;
}

int notmain ( void ) {
	struct serial_channel s;

	uart_init();
    	hexstring(0x12345678);
    	hexstring(GETPC());
    	timer_init();

    	/*
     	 * 132 byte packet.  All fields are 1 byte except for the 128 byte data 
     	 * payload.
     	 * 		+-----+------+----------+--....----+-----+
     	 * 		| SOH | blk# | 255-blk# | ..data.. | crc | 
     	 * 		+-----+------+----------+--....----+-----+
     	 * Protocol:
     	 * 	- first block# = 1.
     	 *  - CRC is over the whole packet 
     	 *  - after all packets sent, sender transmits a single EOT (must ACK).
     	 */
	s = serial_start();
    	while (1) {
		unsigned char b, data[256];

        	// We received an EOT, send an ACK, jump to beginning of code
        	if((b = serial_recv(&s)) == EOT) {
              		uart_send(ACK);
              		BRANCHTO(ARMBASE);
			return 0; // NOTREACHED
		} else if(b == SOH) 
                	s.crc = b;
		else {
			uart_send(NAK);  // Don't recognize command, NAK it
			continue;	 // restart;
		}

        	// Check if first byte of block is expected block number;
        	// if not, NAK, restart for next block
		b = serial_recv(&s);
        	if(b == s.block)
        		s.crc += b;
        	else {
                	uart_send(NAK);
			continue;   // restart
		}

		// Check that second byte of block is negative of block
		// number, if not, NAK, restart for next block
		b = serial_recv(&s);
        	if(b == (0xFF - s.block)) 
			s.crc += b;
        	else {
			uart_send(NAK);
			continue;
		}
	
		// get the data bytes
		int i;
		for(i = 0; i < 128; i++)
                	s.crc += (data[i] = serial_recv(&s));
	
        	// At end of data block, check CRC/checksum (132 byte blocks)
		s.crc &= 0xFF;

		// Checksum failed: NAK the block
		if(serial_recv(&s) != s.crc)
			uart_send(NAK);
		// Checksum passed, copy block to program region.  ACK
		else {
               		for(i = 0; i < 128; i++)
                        	PUT8(s.addr++, data[i]);
			uart_send(ACK);
			s.block = (s.block + 1) & 0xFF;
		}
    	}
    	return(0);
}
