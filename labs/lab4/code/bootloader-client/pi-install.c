#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "demand.h"
#include "serial.h"

static unsigned min(unsigned x, unsigned y) { return x < y ? x : y; }

// read entire file into buffer.  return it.
unsigned char *read_file(int *size, const char *name) {
	int prog = open (name, O_RDONLY);
	if(prog < 0)
        	fatal_err("can't open pi program file: <%s>\n", name);

	// get the size (and other attributes)
	struct stat s;
	if(fstat(prog, &s) < 0)
		fatal_err("can't stat file <%s>\n", name);

	// allocate buffer and read the file contents into it.
	unsigned char *buf = malloc(s.st_size);
	int n = read (prog, buf, s.st_size);
 	if(n != s.st_size) 
        	fatal_err ("bad read file <%s>: got %d bytes expected %lu\n",
		 	name, n, s.st_size);

	close(prog);
	*size = n;
	return buf;
}

enum {
        SOH = 0x01,   // Start Of Header
        ACK = 0x06,   // Acknowledge (positive)
        NAK = 0x15,   // Acknowledge (negative)
        EOT = 0x04,   // End of transmission 
        PAYLOAD_SZ = 128,
};

static void send_byte(int fd, unsigned char b) {
	if(write(fd, &b, 1) < 0)
		fatal_err("write failed in send_byte\n");
}
static unsigned char get_byte(int fd) {
	unsigned char b;
	if(read(fd, &b, 1) != 1)
		fatal_err("read failed in send_byte\n");
	return b;
}


/*
 * tricky part: there is a race condition in that the pi can have a queue
 * of naks sent to us and we will not know necessarily which one we have.
 * so we keep sending the first block until it sends us an ack.
 */
static void send_block(int fd, unsigned blk, const unsigned char *b, int n) {
	unsigned char payload[PAYLOAD_SZ];
	
	// hack so we don't read beyond the buffer at the end.
	memcpy(payload, b, n);

	// keep retransmitting this block until no NAKs
	while(1) {
		send_byte(fd, SOH);
		send_byte(fd, blk);
		send_byte(fd, 0xff - blk);

		unsigned char cksum;
		int i;
		for(cksum = i = 0; i < PAYLOAD_SZ; i++) {
			cksum += payload[i];
			send_byte(fd, payload[i]);
		}
		send_byte(fd, cksum);
		
		unsigned char rx = get_byte(fd);
		// received it
		if(rx == ACK)
			return;
		else if(rx != NAK)
			fatal_err("Invalid reply from pi: <%x>\n", rx);
		else
			printf("xmodem: retransmitting block %d\n", blk);
	}
}

static void xmodem(int fd, const unsigned char * buf, unsigned n) { 
	unsigned nblocks = floor((n+PAYLOAD_SZ-1.0) / (double)PAYLOAD_SZ);
	printf("sending %u bytes (%d blocks) to bootloader\n", n, nblocks);

	int block = 1;
	const unsigned char *p, *e;
	for(p = buf, e = p + n; p < e; p += 128, block++)
		send_block(fd, block, p,  min(128, e - p));

	send_byte(fd, EOT);
	unsigned rx = get_byte(fd);
	if(rx != ACK)
		fatal_err("expected ack, got: %c\n", rx);
		
	printf("sent True\n");
}

/* 
 * I keep getting nailed by problems from binary mistakes.  This catches
 * a couple.
 * This should get expanded: data too large, code too large, stack adjustments
 * too large.  Others?
 *
 * Should allow so you can have multiple prefixes.
 */
static void check_binary(const unsigned char *buf, unsigned n) { 
	// this is the prefix we expect from the pi.  will add a set of these as
	// the programs get more rich.
	static const unsigned char expected_prefix[] = {
        	0x2,
        	0xd9,
        	0xa0,
        	0xe3,
        	0x48,
        	0x1,
        	0x0,
        	0xeb,
        	0xfe,
        	0xff,
        	0xff,
        	0xea,
        	0x0,
        	0x10,
        	0x80,
        	0xe5,
	};

	// others?
	if(memcmp(buf, expected_prefix, sizeof expected_prefix) == 0)
		return;

	puts("ERROR: bad prefix for the pi program.  Possible causes:");
	puts("	* Not doing an objcopy after linking.");
	puts("	* Not linking start as the first routine.");
	puts("Dumping memory:");

	int i;
	for(i = 0; i < sizeof expected_prefix; i++) 
		if(expected_prefix[i] != buf[i]) 
			fprintf(stderr, "\texpected byte[%d] = 0x%x, got 0x%x\n",
				i, expected_prefix[i], buf[i]);

	exit(1);
}
		

int main(int argc, char *argv[]) { 
	if(argc < 2)
		fatal_err("not enough args. usage: pi-install <-p> <device> binary-file\n");

	int arg = 1;
	unsigned print_p = 0;
	if(strcmp(argv[arg], "-p") == 0) {
		print_p = 1;
		arg++;
	}

	char *portname = "/dev/ttyUSB0";
	if((arg + 2) == argc)
		portname = argv[arg++];

	int n;
	unsigned char *buf = read_file(&n, argv[arg]);

	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) 
		fatal_err("opening dev <%s>\n", portname);

	// set speed to 115,200 bps, 8n1 (no parity)
	set_interface_attribs (fd, B115200, 0);  
	set_blocking (fd, 0);                // set no blocking

	check_binary(buf, n);
	xmodem(fd,buf, n);

	// now just keep printing out whatever the pi sends back
	if(!print_p)
		return 0;

	while(1) {
		char buf [4096];

		int n = read (fd, buf, sizeof buf - 1);

		if(!n) {
			struct stat s;
			if(stat(portname, &s) < 0) {
				printf("pi connection closed.  cleaning up\n");
				exit(0);
			}
			usleep(1000);
		} else if(n < 0) {
			printf("pi connection closed.  cleaning up\n");
			exit(0);
		} else {
			buf[sizeof buf - 1] = 0;
			printf("PI: %s", buf);
		}
	}
	return 0;
}
