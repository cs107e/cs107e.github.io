---
title: 'UART to WiFi bridge for two Raspberry Pis'
---

*Written by Chris Gregg*

This tutorial describes how to set up two Raspberry Pi devices to communicate wirelessly
using two ESP32 devices. The Raspberry Pis will communicate over UART, and the ESP32s will
provide a wireless bridge between them. Although not particularly fast, the Raspberry Pis
will act as if they are directly connected via their UART pins. E.g., calling 
`uart_putchar('a')` from one device can be received on the other device with a
corresponding `uart_getchar()`.

Use the following steps to setup the Arduino development environment:

1. Get two ESP-32 devices. This library works for the following Hiletgo devices from Amazon.com (2x for $21.98):[ESP32 on Amazon.com](https://www.amazon.com/HiLetgo-ESP-WROOM-32-Development-Microcontroller-Integrated/dp/B0718T232Z/ref=sr_1_2_sspa)

2. Download our [esp32.zip](/guides/extras/esp32.zip) file and unzip.

3. Download and install the Arduino IDE for your computer: [Arduino Download link](https://www.arduino.cc/en/Main/Software)
    
    On a Mac, copy the IDE program to your `/Applications` folder, and then run it once to install. Then quit the application.

4. Copy the following libraries from the zip folder's `libraries` folder into your Arduino `libraries` folder (for a Mac, the default location is `~/Documents/Arduino/libraries/`):

   `ESP8266-Websocket-master`

   `QueueList`

5. Open up the Arduino application again. Follow the instructions at the following website to install the ESP32 library (under "Testing the Installation", choose the `Node32s` board): [Install ESP32 on Arduino](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-mac-and-linux-instructions/)

6. In the Arduino application, open the `Websocket_server.ino` program from the zip folder. In the program, change the `ssid` variable near the top of the program to be your team name (or another unique WiFi name). You may also change the password for better security.

7. Connect one of the ESP32s to the computer using a micro-USB cable. If you have the USB-serial device connected for your Raspberry Pi, disconnect it first. In the Arduino IDE, select `Tools->Port->SLAB_USBtoUART` (or similar).

8. Click the Upload button in the `Websocket_server | Arduino` window (it is a right-facing arrow). After the "compiling sketch..." message disappears, you should see an "Uploading..." message. If the sketch doesn't automatically upload to the device, you can hold down the button labeled "001" on the ESP32 to force it to listen for the upload (release once it starts uploading).

9. To check to see if the upload worked, open `Tools->Serial Monitor`, select `115200 baud`, and then click the `EN` button on the ESP32. You should see a message that says, "Setting AP (Access Point)_AP IP address: 192.168.4.1". You should also be able to find the `ssid` listed on your phone or computer as a WiFi network to join.

10. Repeat steps 7-9 for your second ESP32, but load the `Websocket_client` program and install it, instead.

11. To test the second ESP32, re-connect your first ESP32. The second ESP32 should find the new WiFi network and automatically connect to the second ESP32, and the blue LED on both devices should turn on to indicate that they are ready to transmit over UART.

For further testing, you can connect two USB-serial connectors to the ESP32s on pins 4 and 5 (labeled `P4` and `P5` on the ESP32 devices), and you can run the screen program on both in separate windows. Keep in mind that you will have multiple `/dev/tty.SLAB_USBtoUART` devices and will need to choose between them. E.g.,

    screen /dev/tty.SLAB_USBtoUART12 115200
    screen /dev/tty.SLAB_USBtoUART 115200

When testing, you should be able to type text into one of the `screen` windows and see it appear in the other `screen` window.
