/* 
 * This code brought to you by the rag tag duo of Arjun and Eric! Visit us at
 * our github accounts: https://github.com/arjunvb, https://github.com/efritz09
 * 
 * Completed on: March 14, 2016
 */

#include "i2c.h"
#include "spi.h"
#include "omni.h"
#include "arducam.h"
#include "gl.h"
#include "printf.h"
#include "malloc.h"

// Image defines
#define WRITE_bm  (1 << 7)
#define WIDTH   320
#define HEIGHT  240

// pixel defines (uses RGB565)
#define BIT_5   0x1F
#define BIT_3   0x07

// SPI parameters
#define POL     0
#define PHA     0
#define CLKDIV  40

//Image handling functions
void get_pixels(unsigned char *rgb);
void print_image(void);
void store_image(void);
void draw_image(void);

//arducam commands
void arducam_write(unsigned char addr, unsigned char value);
unsigned char arducam_read(unsigned char addr);
int arducam_check_interface();
void arducam_set_mode();
void arducam_clear_fifo();
void arducam_begin_capture();
int arducam_capture_done();
unsigned char arducam_chip_version();
unsigned char arducam_read_fifo_single();


typedef struct {
	unsigned height;
	unsigned width;
	unsigned h;
	unsigned w;	//for pixel locations
	unsigned x;
	unsigned y;
	unsigned* start;
} camera_t;

static volatile camera_t cam;

/* This module not only initializes the camera, it also handles the image
 * dislpaying on the monitor, which is why the struct is defined above
 */

//Initializes all the required peripherals, checks the comms, and sets the camera_t values
void arducam_init(unsigned w, unsigned h, unsigned x, unsigned y) {
	spi_init(POL, PHA, CLKDIV);
	printf("spi initialized\n");

	i2c_init();
	printf("i2c initialized\n");

	if (arducam_check_interface()) {
		printf("connected to camera!\n");
	} else {
		printf("SPI interface error\n");
		return;
	}

	omni_init(BMP_MODE);
	arducam_clear_fifo();
	cam.height = h;
	cam.width = w;
	cam.h = y+h;
	cam.w = x+w;
	cam.x = x;
	cam.y = y;
	cam.start = (unsigned*)malloc(h*w);
	printf("--Camera ready--\n");
}

//gets the next pixel value (2 bytes) from the camera and returns a 3 element array
//of RGB values
void get_pixels(unsigned char *rgb)
{
  unsigned char b1 = arducam_read_fifo_single();
  unsigned char b2 = arducam_read_fifo_single();

  unsigned char r = ((b1 >> 3) & BIT_5) * 8; // 5
  unsigned char b = (b2 & BIT_5) * 8; // 5
  unsigned char g = (((b2 >> 5) & BIT_3) | ((b1 & BIT_3) << 3)) * 4; //6
  rgb[0] = r;
  rgb[1] = g;
  rgb[2] = b;
}

//Displays the current camera image to the screen. Does not store the image
void print_image(void)
{
  unsigned char rgb[3];
  for (int i=cam.y; i < cam.h; i++) {
  	if(cam.x == 0) {
	    for (int j=cam.w-1; j >= 0; j--) {
	      get_pixels(rgb);
	      gl_draw_pixel(j, i, gl_color(rgb[0],rgb[1],rgb[2]));
	    }
	} else {
		for (int j=cam.w-1; j >= cam.x; j--) {
	      get_pixels(rgb);
	      gl_draw_pixel(j, i, gl_color(rgb[0],rgb[1],rgb[2]));
	    }
	}
  }
  arducam_clear_fifo();
}

// stores the current image
void store_image(void) {
	color (*im)[cam.width] = (unsigned (*)[cam.width])cam.start;
	unsigned char rgb[3];
	for(int i = cam.y; i < cam.h; i++) {
		for (int j=cam.w-1; j >= cam.x; j--) {
			get_pixels(rgb);
			im[j][i] = gl_color(rgb[0],rgb[1],rgb[2]);
		}
	}
	arducam_clear_fifo();
}

// draws the current image to the display
void draw_image(void) {
	color (*im)[cam.width] = (unsigned (*)[cam.width])cam.start;
	for(int i = cam.y; i < cam.h; i++) {
		for(int j = cam.x; j < cam.w; j++) {
			gl_draw_pixel(j,i,im[j][i]);
		}
	}
}

//calls the commands required to stream the images
void stream_image(void) {
	arducam_begin_capture();
	printf("streaming...\n");

	while(!arducam_capture_done());
	printf("capture done!\n");

	print_image();
	printf("printing\n");
	gl_swap_buffer();
	printf("done reading!\n");
}

//calls the commands to capture and display an image
void capture_image(void) {
	arducam_begin_capture();
	printf("beginning capture...\n");

	while(!arducam_capture_done());
	printf("capture done!\n");

	store_image();
	printf("image stored, printing...\n");
	draw_image();
	printf("image drawn\n");
	gl_swap_buffer();
	draw_image();
	printf("done reading!\n");
}


/* ARDUCAM INTERFACE FUNCTIONS */
// writes values to a register via SPI
void arducam_write(unsigned char addr, unsigned char value)
{
	unsigned char txd[2] = {addr | WRITE_bm, value};
	unsigned char rxd[2] = {0, 0};
	spi_txrx(txd, rxd, 2);
}

//reads values from a register via SPI
unsigned char arducam_read(unsigned char addr)
{
	unsigned char txd[2] = {addr, 0};
	unsigned char rxd[2] = {0, 0};
	spi_txrx(txd, rxd, 2);
	return rxd[1];
}

//tests the SPI connection by sending a dummy value
int arducam_check_interface()
{
	arducam_write(ARD_TEST_REGISTER, TEST_CODE);
	return (arducam_read(ARD_TEST_REGISTER) == TEST_CODE);
}

void arducam_set_mode()
{
	arducam_write(ARD_MODE, MCU2LCD_MODE);
}

void arducam_flush_fifo()
{
	arducam_write(ARD_FIFO_CONTROL, FIFO_CLEAR);
}

void arducam_clear_fifo()
{
	arducam_write(ARD_FIFO_CONTROL, FIFO_CLEAR);
}

void arducam_begin_capture()
{
	arducam_write(ARD_FIFO_CONTROL, FIFO_START);
}

unsigned char arducam_chip_version()
{
	return arducam_read(ARD_CHIP_VERSION);
}

int arducam_capture_done()
{
	return (arducam_read(ARD_CAMERA_STATUS) & FIFO_WRITE_DONE);
}

unsigned char arducam_read_fifo_single()
{
	return arducam_read(ARD_FIFO_SINGLE_READ);
}
