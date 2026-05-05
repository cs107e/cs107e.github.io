In docs directory:
- MCP3008 datasheet
- circuit diagram
- SPI protocol

The MCP3008 is a 8-channel 10-bit analog to digital converter (ADC).
The demo program connects 6 of the channels to various analog inputs.

See 
- https://learn.adafruit.com/raspberry-pi-analog-to-digital-converters/mcp3008
- http://blog.falafel.com/mcp3008-analog-to-digital-conversion

Wire the MCP3008 to the Pi as follows:

MCP3008 VDD to Pi 3.3V
MCP3008 VREF to Pi 3.3V
MCP3008 AGND to Pi GND
MCP3008 DGND to Pi GND
MCP3008 CLK to Pi SPI SCLK
MCP3008 DOUT to Pi SPI MISO
MCP3008 DIN to Pi SPI MOSI
MCP3008 CS/SHDN to Pi SPI CS
