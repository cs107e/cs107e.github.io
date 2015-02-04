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

enum { 
       	SOH = 0x01,   // Start Of Header
       	ACK = 0x06,   // Acknowledge (positive)
       	NAK = 0x15,   // Acknowledge (negative)
       	EOT = 0x04,   // End of transmission 

       	PAYLOAD_SIZE = 128,
	ARMBASE = 0x8000
};

static unsigned char serial_recv(void) { 
	static unsigned rx;
	if(!rx)
		rx = timer_tick();

	// not ready, just keep going.
	while(((uart_lcr() & 0x01) == 0)) {
		// nak every 4 sec
        	if ((timer_tick() - rx) >= 4000000) {
            		uart_send(NAK);
            		rx += 4000000;
        	}
	}
        unsigned char uc = uart_recv();
        rx = timer_tick();
	return uc;
}

int notmain ( void ) {
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
	unsigned char block = 1, crc = 0;
    	unsigned addr = ARMBASE;
    	while (1) {
		unsigned char b, data[256];

        	// We received an EOT, send an ACK, jump to beginning of code
        	if((b = serial_recv()) == EOT) {
              		uart_send(ACK);
              		BRANCHTO(ARMBASE);
			return 0; // NOTREACHED
		} else if(b != SOH) {
			uart_send(NAK);  // Don't recognize command, NAK it
			continue;	 // restart;
		}

        	// Check if first byte of block is expected block number;
        	// if not, NAK, restart for next block
		if(serial_recv() != block) {
                	uart_send(NAK);
			continue;   // restart
		}

		// Check that second byte of block is negative of block
		// number, if not, NAK, restart for next block
		b = serial_recv();
        	if(b == (0xFF - block)) 
			crc = (SOH + block + b);
        	else {
			uart_send(NAK);
			continue;
		}
	
		// get the data bytes
		int i;
		for(i = 0; i < PAYLOAD_SIZE; i++)
                	crc += (data[i] = serial_recv());
	
		// Checksum failed: NAK the block
		if(serial_recv() != crc)
			uart_send(NAK);
		// Checksum passed, copy block to program region.  ACK
		else {
               		for(i = 0; i < PAYLOAD_SIZE; i++)
                        	PUT8(addr++, data[i]);
			uart_send(ACK);
			block++;
		}
    	}
    	return(0);
}
