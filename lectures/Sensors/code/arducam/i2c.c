/* 
 * This code brought to you by the rag tag duo of Arjun and Eric! Visit us at
 * our github accounts: https://github.com/arjunvb, https://github.com/efritz09
 * additionally, check out Eric's personal website! It definitely won't give you 
 * a virus!  http://efritz.xyz
 * 
 * Completed on: March 14, 2016
 */
\
#include "i2c.h"
#include "gpio.h"
#include "printf.h"

typedef struct {
	volatile unsigned* control;
	volatile unsigned* status;
	volatile unsigned* dlen;
	volatile unsigned* fifo;
} i2c_t;

volatile i2c_t i2c;

/* this code assumes you're using the BSC1 channel. To use the others, you'd 
 * want to change the init function to accept a value to select it. Since we
 * only needed one channel, we didn't spend the time to make the driver more
 * general. 
 */

//set the function as alternate
void i2c_init (void) {
	gpio_set_function(I2C_BSC1_SDA,GPIO_FUNC_ALT0);
	gpio_set_function(I2C_BSC1_SCL,GPIO_FUNC_ALT0);
	i2c.control = (unsigned*)I2C_BSC1_CONTROL;
	i2c.status = (unsigned*)I2C_BSC1_STATUS;
	i2c.dlen = (unsigned*)I2C_BSC1_DLEN;
	i2c.fifo = (unsigned*)I2C_BSC1_DATA_FIFO;
}

//sets the slave address in BSC1
void i2c_set_slave (unsigned slave_addr) {
	unsigned* slave_register = (unsigned*)I2C_BSC1_SLAVE_ADDR;
	slave_addr &= I2C_ADDR_MASK; //make sure it is the right size
	*slave_register = slave_addr;
}

//write an array to i2c
void i2c_write (unsigned len, char* data) {
	unsigned index = 0;
	
	//clear fifo, use control.clear field
	*i2c.control |= I2C_CLEAR;

	//clear status clkt, err, and done flags from previous transmissions
	*i2c.status |= (I2C_DONE | I2C_ERR | I2C_CLKT);

	// set it to write
	*i2c.control &= ~I2C_READ_WRITE;

	//set data length
	*i2c.dlen = len;

	//fill the fifo buffer 
	for(; index < I2C_FIFO_SIZE; index++) {
		if(index >= len) break; //make sure not to exceed boundaries
		*i2c.fifo = data[index];
	}

	//enable device and start transfer (read bit = 0)
	*i2c.control |= (I2C_ENABLE | I2C_START);

	//check done status, returns 1 when done
	while((*i2c.status & I2C_DONE) == 0) {
		//check the tx status to see if we can add to the buffer
		while((*i2c.status & I2C_TXD)==1) {
			if(index >= len) break; //make sure not to exceed boundaries
			*i2c.fifo = data[index++];
		}

	}

	//check nack
	if(*i2c.status & I2C_ERR) printf("--NACK received--\n");
	//check clock stretch timeout
	if(*i2c.status & I2C_CLKT) printf("--clock stretch timeout--\n");
	//check data not sent
	if(index < len) printf("--not all data sent--\n");

}

void i2c_read (unsigned len, char* buf) {
	unsigned index = 0;

	//clear fifo
	*i2c.control |= I2C_CLEAR;

	//clear status clkt, err, and done from previous transmission
	*i2c.status |= (I2C_DONE | I2C_ERR | I2C_CLKT);

	//set data len
	*i2c.dlen = len;

	//enable device and start read (set read bit)
	*i2c.control |= (I2C_ENABLE | I2C_START | I2C_READ_WRITE);

	//wait for transfer to complete
	while(!(*i2c.status & I2C_DONE)) {
		//check for a new byte received
		while(*i2c.status & I2C_RXD) {
			//write it to our buffer
			buf[index++] = *i2c.fifo;
		}
	}

	//if the transfer is done, empty the remaining fifo buffer
	while(index < len) {
		while(*i2c.status & I2C_RXD) {
			buf[index++] = *i2c.fifo;
		}
	}
	
	//check nack
	if(*i2c.status & I2C_ERR) printf("--NACK received--\n");
	//check clock stretch timeout
	if(*i2c.status & I2C_CLKT) printf("--clock stretch timeout--\n");
	//check data not sent
	if(index < len) printf("--not all data sent--\n");
}
