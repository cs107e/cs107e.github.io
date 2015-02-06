/* 
 * engler: Rewritten version of Welch's bootloader.
 * Copyright (c) 2012 David Welch dwelch@dwelch.com
 *
 * The raspberry pi firmware at the time this was written defaults
 * loading at address 0x8000.  Although this bootloader could easily
 * load at 0x0000, it loads at 0x8000 so that the same binaries built
 * for the SD card work with this bootloader.  Change the ARMBASE
 * below to use a different location.
 */
#include "rpi.h"

enum { 
       	SOH = 0x01,   // Start Of Header
       	ACK = 0x06,   // Acknowledge (positive)
       	NAK = 0x15,   // Acknowledge (negative)
       	EOT = 0x04,   // End of transmission 
       	PAYLOAD_SIZE = 128,

	ARMBASE = 0x8000
};

static unsigned char getbyte(void) { 
	unsigned t0 = timer_tick();
	// while uart not ready, just keep going; nak every 4 sec
	while(((uart_lcr() & 0x01) == 0)) {
		unsigned t = timer_tick();
        	if ((t - t0) >= 4000000) {
            		uart_send(NAK);
            		t0 = t;
        	}
	}
        return uart_recv();
}

void notmain ( void ) {
	uart_init();
    	hexstring(0x12345678);
    	hexstring(GETPC());
    	timer_init();

    	/*
     	 * 132 byte packet.  All fields are 1 byte except for the 128 byte data 
     	 * payload.
     	 * 		+-----+------+----------+--....----+-----+
     	 * 		| SOH | blk# | 255-blk# | ..data.. | cksum | 
     	 * 		+-----+------+----------+--....----+-----+
     	 * Protocol:
     	 * 	- first block# = 1.
     	 *  - CRC is over the whole packet 
     	 *  - after all packets sent, sender transmits a single EOT (must ACK).
     	 */
	unsigned char block = 1;
    	unsigned addr = ARMBASE;
    	while (1) {
		unsigned char b;

        	// We received an EOT, send an ACK, jump to beginning of code
        	if((b = getbyte()) == EOT) {
              		uart_send(ACK);
              		BRANCHTO(ARMBASE);
			return; // NOTREACHED
		}

		/* 
		 * if first byte is not SOH, or second byte is not the 
		 * expected block number or the third byte is not its
		 * negation, send a nak for a resend of this block.
		 */
		if(b != SOH 
		|| getbyte() != block 
		|| getbyte() != (0xFF - block)) {
                	uart_send(NAK);
			continue;  
		}

		// get the data bytes
		int i;
		unsigned char cksum;
		for(cksum = i = 0; i < PAYLOAD_SIZE; i++) {
                	cksum += (b = getbyte());
                        PUT8(addr+i, b);
		}
	
		// Checksum failed: NAK the block
		if(getbyte() != cksum)
			uart_send(NAK);
		// Commit our addr pointer and go to next block.
		else {
			uart_send(ACK);
			addr += PAYLOAD_SIZE;
			block++;
		}
    	}
}
