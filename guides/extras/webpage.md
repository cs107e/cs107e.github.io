---
title: Using an ESP-32 to create a web page
---

*Written by Chris Gregg*

This tutorial describes how to set up a rudimentary web page on an ESP-32.

Use the following steps to setup the Arduino development environment:

1. Get one ESP-32 arduino device. This library works for the following Hiletgo devices from Amazon.com (2x for $21.98): [ESP32 on Amazon.com](https://www.amazon.com/HiLetgo-ESP-WROOM-32-Development-Microcontroller-Integrated/dp/B0718T232Z/ref=sr_1_2_sspa)

2. Download our [esp32.zip](/guides/extras/esp32.zip) file and unzip.

3. Download and install the Arduino IDE for your computer: [Arduino Download link](https://www.arduino.cc/en/Main/Software)
    
    On a Mac, copy the IDE program to your `/Applications` folder, and then run it once to install. Then quit the application.

4. Copy the following libraries from the zip folder's `libraries` folder into your Arduino `libraries` folder (for a Mac, the default location is `~/Documents/Arduino/libraries/`):

   `ESP8266-Websocket-master`

   `QueueList`

5. Open up the Arduino application again. Follow the instructions at the following website to install the ESP32 library (under "Testing the Installation", choose the `Node32s` board):[Install ESP32 on Arduino](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-mac-and-linux-instructions/)

6. In the Arduino application, open the `MIDI_web_example.ino` program from the zip folder. In the program, change the `ssid` variable near the top of the program to be your team name (or another unique WiFi name). You may also change the password for better security.

7. Connect your ESP32 to the computer using a micro-USB cable. If you have the USB-serial device connected for your Raspberry Pi, disconnect it first. In the Arduino IDE, select `Tools->Port->SLAB_USBtoUART` (or similar).

8. Click the Upload button in the `MIDI_web_example | Arduino` window (it is a right-facing arrow). After the "compiling sketch..." message disappears, you should see an "Uploading..." message. If the sketch doesn't automatically upload to the device, you can hold down the button labeled "001" on the ESP32 to force it to listen for the upload (release once it starts uploading).

9. To check to see if the upload worked, open `Tools->Serial Monitor`, select `115200 baud`, and then click the `EN` button on the ESP32. You should see a message that says, "Setting AP (Access Point)_AP IP address: 192.168.4.1". You should also be able to find the `ssid` listed on your phone or computer as a WiFi network to join.

10. At this point, you should be able to connect to the WiFi network you named in step (6). Once you connect from a device, go to the web page `192.168.4.1` and you should see the web page that says, "Play some notes!"

If you want to load a web page from your SD card, you can use the sd library, which is described in a [different guide](../sd_library/).
