#ifndef I2C_H
#define I2C_H

//BSC0 register 
#define CONTROL_REGISTER_BSC0	0x20205000
#define I2C_BSC0_CONTROL		0x20205000
#define I2C_BSC0_STATUS			0x20205004
#define I2C_BSC0_DLEN			0x20205008
#define I2C_BSC0_SLAVE_ADDR		0x2020500C
#define I2C_BSC0_DATA_FIFO		0x20205010
#define I2C_BSC0_CLK_DIV		0x20205014
#define I2C_BSC0_DATA_DELAY		0x20205018
#define I2C_BSC0_CLK_TIMEOUT	0x2020501C

//BSC1 register 
#define CONTROL_REGISTER_BSC1	0x20804000
#define I2C_BSC1_CONTROL		0x20804000
#define I2C_BSC1_STATUS			0x20804004
#define I2C_BSC1_DLEN			0x20804008
#define I2C_BSC1_SLAVE_ADDR		0x2080400C
#define I2C_BSC1_DATA_FIFO		0x20804010
#define I2C_BSC1_CLK_DIV		0x20804014
#define I2C_BSC1_DATA_DELAY		0x20804018
#define I2C_BSC1_CLK_TIMEOUT	0x2080401C

#define I2C_BSC1_SDA			2 //gpio 2, alt0
#define I2C_BSC1_SCL			3 //gpio 3, alt0

//BSC0 CONTROL REGISTER, pg 29
#define I2C_READ_WRITE	0x01
#define I2C_CLEAR 	(0x01 << 4)
#define I2C_START 	(0x01 << 7)
#define I2C_INTD	(0x01 << 8) //interrupt on DONE
#define I2C_INTT	(0x01 << 9)	//interrupt on TX
#define I2C_INTR	(0x01 << 10)	//interrupt on RX
#define I2C_ENABLE	(0x01 << 15)
#define I2C_DISABLE	(0x00 << 15) //kinda dumb, I know

//BSC0 STATUS REGISTER, pg 31
#define I2C_TA 		0x01 //read only
#define I2C_DONE	(0x01 << 1) //clears INTD
#define I2C_TXW		(0x01 << 2) //RO
#define I2C_RXR		(0x01 << 3) //RO
#define I2C_TXD		(0x01 << 4) //RO
#define I2C_RXD		(0x01 << 5) //RO
#define I2C_TXE		(0x01 << 6) //RO, reset=0x01
#define I2C_RXF		(0x01 << 7) //RO
#define I2C_ERR		(0x01 << 8) //0 = no errors, clear by writing 1
#define I2C_CLKT 	(0x01 << 9)	//0 = no errors


//BSC0 DATA LENGTH REGISTER, pg 32
#define I2C_DLEN 	0X00 //RW, bytes remaining in current transfer

//BSC0 SLAVE ADDRESS REGISTER, pg 33
#define I2C_ADDR 	0x00 //7 bits
#define I2C_ADDR_MASK	0x7F

//BSC0 FIFO REGISTER, pg 33
#define I2C_DATA 	0x00 //8 bits, cleared using i2c.c.clear
#define I2C_FIFO_SIZE	16

//BSC0 CLOCK DIVIDER REGISTER, pg 34
#define I2C_CDIV 	0x00 //2 bytes, SCL = core clock/CDIV
						 //reset = 0x5DC

//BSC0 DATA DELAY REGISTER, pg 34
#define I2C_REDL	0x00 //Rising edge delay, reset=0x30
#define I2C_FEDL	(0x01<<16) //falling edge delay, reset=0x30

//BSC0 CLOCK TIMEOUT REGISTER, pg 35
#define I2C_TOUT 	0x00 //2 bytes, reset=0x40

//Interrupt control
#define INT_ENABLE_2            0x2000B214
#define INT_DISABLE_2           0x2000B220
#define I2C_INTERRUPT			(0x01 << 21)
#define I2C_SPI_SLV_INTERRUPT	(0x01 << 11)

void i2c_init (void);
void i2c_set_slave (unsigned slave_addr);
void i2c_write (unsigned len, char* data);
void i2c_read (unsigned len, char* buf);
void i2c_end(void);
/*
void i2c_begin(unsigned slave_addr);
void i2c_add_byte(char byte);
void i2c_transmit(void);
*/

#endif
