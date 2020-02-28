#ifndef OMNI_H
#define OMNI_H

/* This code brought to you by the rag tag duo of Arjun and Eric! Visit us at
 * our github accounts: https://github.com/arjunvb, https://github.com/efritz09
 */

//Omnivision address for the OV2640 camera (check with I2C)
#define OV_ADDR1	0x26
#define OV_ADDR2	0x42

//Camera modes
#define BMP_MODE	0xAB
#define JPG_MODE	0xEF

//functions
void omni_init(unsigned mode);

//different preprogrammed modes (commands found starting on page 14 of 
//			Camera Module Software Applications Notes datasheet)
void omni_set_normal();
void omni_set_antique();
void omni_set_reddish();
void omni_set_greenish();
void omni_set_bluish();
void omni_set_bw();
void omni_set_neg();
void omni_set_bw_neg();
void omni_set_cloudy();
void omni_set_auto();
void omni_set_sunny();
void omni_set_office();
void omni_set_home();
void omni_set_saturation(int saturation);
void omni_set_contrast(int contrast);
void omni_set_brightness(int brightness);

//ov2640 registers (pg 18 of OV2640DS datasheet)
#define OV_R_BYPASS	0x05
#define OV_QS		0x44
#define OV_CTRLI	0x50
#define OV_HSIZE	0x51
#define OV_VSIZE	0x52
#define OV_XOFFL	0x53
#define OV_YOFFL	0x54
#define OV_VHYX		0x55
#define OV_DPRP		0x56
#define OV_TEST		0x57
#define OV_ZMOW		0x5A
#define OV_ZMOH		0x5B
#define OV_ZMHH		0x5C
#define OV_BPADDR	0x7C
#define OV_BPDATA	0x7D
#define OV_CTRL2	0x86
#define OV_CTRL3	0x87
#define OV_SIZEL	0x8C
#define OV_HSIZE8	0xC0
#define OV_VSIZE8	0xC1
#define OV_CTRL0	0xC2
#define OV_CTRL1	0xC3
#define OV_R_DVP_SP	0xD3
#define OV_IMAGE_MODE	0xDA
#define OV_RESET	0xE0
#define OV_MS_SP	0xF0
#define OV_SS_ID	0xF7
#define OV_SS_CTRL	0xF8
#define OV_MC_BIST	0xF9
#define OV_MC_AL	0xFA
#define OV_MC_AH	0xFB
#define OV_MC_D		0xFC
#define OV_P_CMD	0xFD
#define OV_P_STATUS	0xFE
#define OV_RA_DLMT	0xFF

//registers IF 0xFF == 01 (pg 22)
#define OV_GAIN		0x00
#define OV_COM1		0x03
#define OV_REG04	0x04
#define OV_REG08	0x08
#define OV_COM2		0x09
#define OV_PIDH		0x0A
#define OV_PIDL		0x0B
#define OV_COM3		0x0C
#define OV_COM4		0x0D
#define OV_AEC		0x10
#define OV_CLKRC	0x11
#define OV_COM7		0x12
#define OV_COM8		0x13
#define OV_COM9		0x14
#define OV_COM10	0x15
#define OV_HREFST	0x17
#define OV_HREFEND	0x18
#define OV_VSTRT	0x19
#define OV_VEND		0x1A
#define OV_MIDH		0x1C
#define OV_MIDL		0x1D
#define OV_AEW		0x24
#define OV_AEB		0x25
#define OV_VV		0x26
#define OV_REG2A	0x2A
#define OV_FRARL	0x2B
#define OV_ADDVSL	0x2D
#define OV_ADDVSH	0x2E
#define OV_YAVG		0x2F
#define OV_HSDY		0x30
#define OV_HEDY		0x31
#define OV_REG32	0x32
#define OV_ARCOM2	0x34
#define OV_REG45	0x45
#define OV_FLL		0x46
#define OV_FLH		0x47
#define OV_COM19	0x48
#define OV_ZOOMS	0x49
#define OV_COM22	0x4B
#define OV_COM25	0x4E
#define OV_BD50		0x4F
#define OV_BD60		0x50
#define OV_REG5D	0x5D
#define OV_REG5E	0x5E
#define OV_REG5F	0x5F
#define OV_REG60	0x60
#define OV_HISTO_LOW	0x61
#define OV_HISTO_HIGH	0x62


#endif