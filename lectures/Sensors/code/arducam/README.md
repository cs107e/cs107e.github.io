#Interfacing an ArduCAM with the Raspberry Pi

![][hoover-tower]

##Team Members
Arjun Balasingam and Eric Fritz

###Roles
The following is a breakdown of the tasks we each took on, while working on this project:
- Arjun: SPI driver, ArduCAM Library, Image Display and Modes
- Eric: I2C driver, OV2640 Library, Shell

Note that we both cross-checked and debugged all of the code, so we were confident it is robust.


###Overview
The goal of this project was to **interface a camera** with the Raspberry Pi, and **develop a library** that makes it convenient to access the various features that the camera offers. To demonstrate these features, we put together a shell, where one can enter commands and see the image output rendered on the screen.

The selected camera was an [ArduCAM Mini][arducam-mini], a 2 MP camera designed to work with the [Arduino][arduino]. This camera was chosen because the hardware was designed to be easily interfaced with microcontrollers. The ArduCAM utilizes an Omnivision CMOS Image Sensor [OV2640][ov2640-ds]. It communicates over SPI and I2C, two protocols that are supported on Raspberry Pi hardware.

The sections below describe the components of the camera library we built, including SPI and I2C drivers, an image sensor driver, and a camera driver.


###Communication Drivers
The ArduCAM reqires both SPI and I2C to control the image sensor and recieve images from the camera. SPI was used to send image capture commands as well as transmit the raw pixel or JPEG data. I2C was used to change the Omnivision sensor's registers directly, allowing the user to adjust the camera settings.

Our [SPI driver][spi-driver] consists of the following functions:
- `spi_init`: Initializes the RPi SPI0 peripheral with an inputted clock `pol` (polarity), `pha` (phase), and `clk_div` (scale).
- `spi_txrx`: Transmits tx_buf of length `len` bytes. Stores received contents into `rx_buf`.

Our [I2C driver][i2c-driver] consists of the following functions:
- `i2c_init`: Initializes the RPi I2C peripheral.
- `i2c_set_slave`: Sets the slave address to be `slave_addr`.
- `i2c_write`: Writes a buffer `data` of length `len` over I2C.
- `i2c_read`: Reads `len` bytes over I2C. Stores the conents in `buf`.

###Wiring Up the ArduCAM to the RPi
The following pictures show how we wired the ArduCAM up to the RPi:

![][arducam-front]
![][wiring]


###Device Drivers
After we implemented and tested the two communications protocols, we created a [driver][arducam-library] for the camera to interact with the RPi over SPI and I2C.

Our first step was determining the specifics of SPI and I2C that were required to communicated directly with camera. For instance, writing to an ArduCAM register involved sending an address byte, followed by a data byte. Since the most significant bit of the address must be set in order to write to that particular register, we bitwise-or the address with `0x80`. To read a register value, we send the address on the first byte, ignore the first read, send a dummy value on the second byte, and read that incoming value. 

With these two read and write functionalities, we wrote several functions that wrote to and read from ArduCAM registers. These functions were all folded into `arducam_init`, which initializes and verifies the SPI connection, and cleared the FIFO. `stream_image` captures continously, rendering a live camera stream to the screen. `capture_image` initiates and executess a single capture, and stores that image in a buffer.

Within the ArduCAM library, we integrated a [driver][omni-library] for the Omnivision image sensor. `omni_init` initializes and verfies an I2C connection, by reading the sensor's address. It also prepares the image sensor to return image output in `mode` format (BMP or JPEG). Finally, we wrote a set of functions to control brightness, contrast, saturation, and special effect parameters that the image sensor offers. The I2C commands we followed are listed in the [OV2640 Software Applications Notes][ov2640-software].

We chose to use the QVGA format for our driver due to the fact that it sends bitmap pixel data and that it's a small resolution (360 x 240). We didn't spend any time optimizing our graphics library so larger images took too long to display.

To make the process interactive and be able demonstrate the functionalities provided by our camera library, we implemented a shell that executed the commands from user input and updated the display to show the results.


###Debugging and Testing
Throughout this project, the most useful tool for debugging was a logic analyzer. We made incremental changes to the SPI and I2C drivers, making sure each time that the correct test messages were being sent. Our largest roadblock was not being able to detect the end of a single capture (by polling a bit on a register). The ArduCAM datasheet incorrectly stated the SPI clock phase and polarity. We were able to verify this by comparing the timing diagram on the datasheet (which was correct) with clock (SCLK) traces from the logic analyzer.


###Camera Library and Features
The shell has a stream mode which updates the image as fast as our graphics library allows, and a capture mode, which captures and displays a single image. The library contained functions to adjust the brightness, contrast, and saturation, each of which had 5 options to choose (-2, 2). There is also a handful of pre-programmed "filters" such as color biasing, negative, and greyscale. A complete list can be found in [omni.h][omni] with the commands required in [omni_cmds.h][omni]. 

![][image-display]

###Collaboration and Help Received
During the SPI clock phase/polarity issue, we heavily relied on the [ArduCAM's arduino code][arducam-git] to debug. The omnivision datasheets were very useful as well. 


[arducam-mini]: http://www.arducam.com/tag/arducam-mini/
[arduino]: https://www.arduino.cc/
[ov2640-ds]: Datasheets/OV2640DS.pdf
[ov2640-software]: Datasheets/OV2640%20Camera%20Module%20Software%20Application%20Notes.pdf
[arducam-git]: https://github.com/ArduCAM/Arduino
[shell]: src/shell.c
[omni]: inc/omni_cmds.h
[spi-driver]: inc/spi.h
[i2c-driver]: inc/i2c.h
[arducam-library]: inc/arducam.h
[omni-library]: inc/omni.h
[arducam-front]: photos/arducam_front.JPG
[hoover-tower]: photos/hoover_tower.png
[wiring]: photos/wiring.JPG
[image-display]: photos/image_display.png
