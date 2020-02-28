The MCP3008 is a 8-channel 10-bit analog to digital converter (ADC). 

See 
- https://learn.adafruit.com/raspberry-pi-analog-to-digital-converters/mcp3008
- http://blog.falafel.com/mcp3008-analog-to-digital-conversion

Wire the MCP3008 to the Raspberry Pi as follows:

MCP3008 VDD to Raspberry Pi 3.3V
MCP3008 VREF to Raspberry Pi 3.3V
MCP3008 AGND to Raspberry Pi GND
MCP3008 DGND to Raspberry Pi GND
MCP3008 CLK to Raspberry Pi SCLK
MCP3008 DOUT to Raspberry Pi MISO
MCP3008 DIN to Raspberry Pi MOSI
MCP3008 CS/SHDN to Raspberry Pi CE0

doc/ contains

- datasheet
- circuit diagram
- spi protocol


