The MCP3008 is a 8-channel 10-bit analog to digital converter (ADC). 

It is set up to read the x,y coordinates from an analog joystick.

See 
- https://learn.adafruit.com/raspberry-pi-analog-to-digital-converters/mcp3008
- http://blog.falafel.com/mcp3008-analog-to-digital-conversion

Wire the MCP3008 to the Pi as follows:

MCP3008 VDD to Pi 3.3V
MCP3008 VREF to Pi 3.3V
MCP3008 AGND to Pi GND
MCP3008 DGND to Pi GND
MCP3008 CLK to Pi SCLK
MCP3008 DOUT to Pi MISO
MCP3008 DIN to Pi MOSI
MCP3008 CS/SHDN to Pi CS

doc/ contains

- datasheet
- circuit diagram
- spi protocol


