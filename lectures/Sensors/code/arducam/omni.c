/* 
 * This code brought to you by the rag tag duo of Arjun and Eric! Visit us at
 * our github accounts: https://github.com/arjunvb, https://github.com/efritz09
 * 
 * Completed on: March 14, 2016
 */

#include "omni.h"
#include "arducam.h"
#include "omni_cmds.h"
#include "i2c.h"
#include "printf.h"

//Sends the commands in order from the blocks defined in omni_cmds.h
void send_command_block(const char *commands)
{
	i2c_set_slave(ARD_I2C_WRITE >> 1);
    unsigned reg_addr = 0;
    unsigned reg_val = 0;
    char data[2];
    while((reg_addr != 0xFF) | (reg_val != 0xFF)) {
	    reg_addr = commands[0];
	    reg_val = commands[1];
	    data[0] = reg_addr;
	    data[1] = reg_val;
	    i2c_write(2,data);
	    commands+=2;
    }
}

//prepares to send a block of commands, as defined in the arducam source code
int prep_omni_commands(void) 
{
    i2c_set_slave(ARD_I2C_WRITE >> 1);
    char data[2];
    data[0] = 0xFF;
    data[1] = 0x01;
    i2c_write(2,data);
    data[0] = 0x12;
    data[1] = 0x80;
    i2c_write(2,data);
    return 1;
}

//writes the initialization commands, with a defined resolution (mode)
int write_omni_commands(unsigned mode) 
{
    const char *start;
    char data[2];

    if (mode == JPG_MODE) {
        start = &OV2640_JPEG_INIT[0][0];
        prep_omni_commands();
        send_command_block(start);
        start = &OV2640_YUV422[0][0];
        prep_omni_commands();
        send_command_block(start);
        start = &OV2640_JPEG[0][0];
        prep_omni_commands();
        send_command_block(start);
        data[0] = 0xFF;
        data[1] = 0x01;
        i2c_write(2,data);
        data[0] = 0x15;
        data[1] = 0x00;
        i2c_write(2,data);
        start = &OV2640_160x120_JPEG[0][0];
        prep_omni_commands();
        send_command_block(start);
    } 
    else if(mode == BMP_MODE) {
        start = &OV2640_QVGA[0][0];
        prep_omni_commands();
        send_command_block(start);

    } 
    else {
        printf("unknown format\n");
        return 0;
    }
 
    printf("Omni commands written\n");
    return 1;
}

//sends query commands to test the i2c link
void omni_init(unsigned mode) 
{
    char buf[2];
    char ret[1];
    buf[0] = OV_RA_DLMT;
    buf[1] = 0x01;  //allows for second register control
    i2c_set_slave(ARD_I2C_WRITE>>1);
    //check the address to ensure we have the right camera
    buf[0] = OV_PIDH;
    i2c_write(1,buf);
    i2c_read(1,ret);
    if(ret[0] != 0x26) {
    	printf("Reading 0x%x. Expected 0x26.\n", ret[0]);
        return;
    }

    //check the second value
    buf[0] = OV_PIDL;
    i2c_write(1,buf);
    i2c_read(1,ret);
    if(ret[0] != 0x42) {
    	printf("Reading 0x%x. Expected 0x42.\n", ret[0]);
        return;
    }
    //if all clear, send the initializationi codes
    write_omni_commands(mode);
}


/***************************** SPECIAL EFFECTS *****************************/
void omni_set_normal()
{
	send_command_block(&NORMAL[0][0]);
}

void omni_set_neg()
{
  send_command_block(&NEG[0][0]);
}

void omni_set_bw()
{
  send_command_block(&BW[0][0]);
}

void omni_set_antique()
{
  send_command_block(&ANTIQUE[0][0]);
}

void omni_set_bw_neg()
{
  send_command_block(&BW_NEG[0][0]);
}

void omni_set_reddish()
{
  send_command_block(&REDDISH[0][0]);
}

void omni_set_bluish()
{
  send_command_block(&BLUISH[0][0]);
}

void omni_set_greenish()
{
  send_command_block(&GREENISH[0][0]);
}

void omni_set_cloudy()
{
    send_command_block(&CLOUDY[0][0]);
}

void omni_set_auto()
{
    send_command_block(&AUTO[0][0]);
}

void omni_set_sunny()
{
    send_command_block(&SUNNY[0][0]);
}

void omni_set_office()
{
    send_command_block(&OFFICE[0][0]);
}

void omni_set_home()
{
    send_command_block(&HOME[0][0]);
}

void omni_set_saturation(int saturation)
{
    printf("saturation = %d\n",saturation);
	switch (saturation) {
		case -2:
			send_command_block(&SATURATION_NEG2[0][0]);
			break;
		case -1:
			send_command_block(&SATURATION_NEG1[0][0]);
			break;
		case 0:
			send_command_block(&SATURATION_ZERO[0][0]);
			break;
		case 1:
			send_command_block(&SATURATION_POS1[0][0]);
			break;
		case 2:
			send_command_block(&SATURATION_POS2[0][0]);
			break;
        default :
            printf("saturation value not recognized\n");
            break;
	}
}

void omni_set_contrast(int contrast)
{
    printf("contrast = %d\n",contrast);
	switch (contrast) {
		case -2:
			send_command_block(&CONTRAST_NEG2[0][0]);
			break;
		case -1:
			send_command_block(&CONTRAST_NEG1[0][0]);
			break;
		case 0:
			send_command_block(&CONTRAST_ZERO[0][0]);
			break;
		case 1:
			send_command_block(&CONTRAST_POS1[0][0]);
			break;
		case 2:
			send_command_block(&CONTRAST_POS2[0][0]);
			break;
        default :
            printf("contrast value not recognized\n");
            break;
	}
}

void omni_set_brightness(int brightness)
{
    printf("brightness = %d\n",brightness);
	switch (brightness) {
		case -2:
			send_command_block(&BRIGHTNESS_NEG2[0][0]);
			break;
		case -1:
			send_command_block(&BRIGHTNESS_NEG1[0][0]);
			break;
		case 0:
			send_command_block(&BRIGHTNESS_ZERO[0][0]);
			break;
		case 1:
			send_command_block(&BRIGHTNESS_POS1[0][0]);
			break;
		case 2:
			send_command_block(&BRIGHTNESS_POS2[0][0]);
			break;
        default :
            printf("brightness value not recognized\n");
            break;
	}
}