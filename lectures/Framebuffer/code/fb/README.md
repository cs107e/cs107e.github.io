Configure the frame buffer and print out its configuration
over the serial port.

Pitch: It appears rows in the framebuffer are aligned to multiples of 64.
Changing the width from 640 to 625 maintains the same pitch (640x4=2560).
