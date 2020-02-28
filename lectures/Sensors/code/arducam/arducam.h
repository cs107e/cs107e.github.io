#ifndef ARDUCAM_H
#define ARDUCAM_H

/* This code brought to you by the rag tag duo of Arjun and Eric! Visit us at
 * our github accounts: https://github.com/arjunvb, https://github.com/efritz09
 */

//arducam SPI register addresses
#define ARD_TEST_REGISTER	    0x00
#define ARD_CAPTURE_CTRL	    0x01
#define ARD_SENSE_TIMING	    0x03
#define ARD_FIFO_CONTROL	    0x04
#define ARD_GPIO_DIRECTION	    0x05
#define ARD_GPIO_WRITE		    0x06
#define ARD_FIFO_BURST_READ	    0x3C
#define ARD_FIFO_SINGLE_READ    0x3D
#define ARD_CHIP_VERSION	    0x40
#define ARD_CAMERA_STATUS	    0x41
#define ARD_CAMERA_WRITE_FIFO_1	0x42
#define ARD_CAMERA_WRITE_FIFO_2	0x43
#define ARD_CAMERA_WRITE_FIFO_3	0x44
#define ARD_GPIO_READ		    0x45

#define TEST_CODE 0x55

// SPI commands
#define FIFO_CLEAR              (1 << 0)
#define FIFO_START              (1 << 1)
#define FIFO_RESET_WRITE        (1 << 2)
#define FIFO_RESET_READ         (1 << 3)
#define FIFO_WRITE_DONE         (1 << 3)

// Special registers (may not be needed)
#define ARD_CAPTURE_COMMAND	    0x84
#define ARD_MODE				0x02
#define MCU2LCD_MODE            0x00

//i2c addresses (since they're >> 1, they're identical...
//				 The datasheet is terrible)
#define ARD_I2C_WRITE	        0x60
#define ARD_I2C_READ	        0x61


// functions
void arducam_init(unsigned w, unsigned h, unsigned x, unsigned y);
void stream_image(void);
void capture_image(void);

#endif