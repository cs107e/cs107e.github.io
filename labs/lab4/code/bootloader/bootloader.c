
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// The raspberry pi firmware at the time this was written defaults
// loading at address 0x8000.  Although this bootloader could easily
// load at 0x0000, it loads at 0x8000 so that the same binaries built
// for the SD card work with this bootloader.  Change the ARMBASE
// below to use a different location.

#define ARMBASE 0x8000

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern unsigned int GETPC ( void );
extern void BRANCHTO ( unsigned int );
extern void dummy ( unsigned int );

extern void uart_init ( void );
extern unsigned int uart_lcr ( void );
extern void uart_send ( unsigned int );
extern unsigned int uart_recv ( void );
extern void hexstring ( unsigned int );
extern void hexstrings ( unsigned int );
extern void timer_init ( void );
extern unsigned int timer_tick ( void );

extern void timer_init ( void );
extern unsigned int timer_tick ( void );

enum { 
       SOH = 0x01,   // Start Of Header
       ACK = 0x06,   // Acknowledge (positive)
       NAK = 0x15,   // Acknowledge (negative)
       EOT = 0x04,   // End of transmission 
       PACKET_SIZE = 132
};

//------------------------------------------------------------------------
unsigned char xstring[256];
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    //unsigned int rb;
    unsigned int rx;
    unsigned int addr;
    unsigned int block;
    unsigned int state; // Stores which byte in block we're on

    unsigned int crc;  // Checksum

    uart_init();
    hexstring(0x12345678);
    hexstring(GETPC());
    timer_init();

//block numbers start with 1

//132 byte packet
//starts with SOH
//block number byte
//255-block number
//128 bytes of data
//checksum byte (whole packet)
//a single EOT instead of SOH when done, send an ACK on it too

    block = 1;
    addr = ARMBASE;
    state = 0;
    crc = 0;
    rx = timer_tick();
    while (1)
    {
        ra = timer_tick();
        if ((ra - rx) >= 4000000)
        {
            uart_send(NAK);
            rx += 4000000;
        }
        // What does this do?
        if ((uart_lcr() & 0x01) == 0) continue;
        xstring[state] = uart_recv();
        rx = timer_tick();
        if (state == 0)
        {
          // We received an EOT, send an ACK, jump to beginning of code
          if(xstring[state] == EOT)
            {
              uart_send(ACK);
              BRANCHTO(ARMBASE);
              break;
            }
        }
        switch(state)
        {
            case 0:   // Beginning of block
            {
                if(xstring[state] == SOH)  // Start of new block
                {
                    crc = xstring[state];
                    state++;
                }
                else
                {
                    //state=0;
                    uart_send(NAK);  // Don't recognize command, NAK it
                }
                break;
            }
            case 1:
            {
                // Check if first byte of block is expected block number;
                // if not, NAK, restart for next block
                if(xstring[state] == block) 
                {
                    crc += xstring[state];
                    state++;
                }
                else
                {
                    state = 0;
                    uart_send(NAK);
                }
                break;
            }
            case 2:
            {
                // Check that second byte of block is negative of block
                // number, if not, NAK, restart for next block
                if(xstring[state] == (0xFF - xstring[state - 1]))
                {
                    crc += xstring[state];
                    state++;
                }
                else
                {
                    uart_send(NAK);
                    state = 0;
                }
                break;
            }
            // At end of data block, check CRC/checksum (132 byte blocks)
            case (PACKET_SIZE - 1): 
            {
                crc &= 0xFF;
                if(xstring[state] == crc)
                {
                    // Checksum passed, copy block to program region,
                    // acknowledge, get ready for next block
                    for(ra = 0; ra < 128; ra++)
                    {
                        PUT8(addr++, xstring[ra + 3]);
                    }
                    uart_send(ACK);
                    block = (block + 1) & 0xFF;
                }
                // Checksum not passed, NAK the block
                else 
                {
                    uart_send(NAK);
                }
                state = 0;
                break;
            }
            // Base case for data bytes: update checksum
            default:
            {
                crc += xstring[state];
                state++;
                break;
            }
        }
    }
    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2012 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
