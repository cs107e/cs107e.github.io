#ifndef RPI_LOGO_H
#define RPI_LOGO_H

int rpi_logo_height();
int rpi_logo_width();
int rpi_logo_byte_depth();

/* Returns the image as a buffer with the above dimensions. */
char* rpi_logo_image();

#endif
