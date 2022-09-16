# twilio-arduino
Example code to use a LilyGO-T-SIM7000G in conjunction with various Twilio APIs

## Useful Links
* LILYGO sample code: https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G
* Basics on using Arduino IDE: https://docs.arduino.cc/software/ide-v1/tutorials/arduino-ide-v1-basics
* Tips on programming for the Arduino: https://www.programmingelectronics.com/tutorial-3-arduino-ide-and-sketch-overview/

## IDE Setup for MacOS

Instructions on setting up the Arduino environment for use with the LilyGO-T-SIM7000G

> Tested configuration:  MacBook Pro (16inch, 2019), MacOS Monterey 12.5.1

### Download and install Arduino IDE 1.8.x  
This is the basic development environment for compiling and loading code on any Arduino board.
* https://www.arduino.cc/en/software
* Download and install v 1.8.x

### Install ESP32 tools.  
These tools are specific to the ESP32 microcontroller that is on the LILYGO board.

* General Instructions: https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html

* Specific Instructions:
  1. Open the Arduino IDE
  2. Open Preferences panel
  3. Look for “Additional Boards Manager” near bottom of Preferences panel
  4. Enter this URL: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

### Install ESP32 board configuration dependencies in Arduino IDE
This will load configuration specific information for compiling code for the ESP32.

  1. Open Arduino IDE
  2. Open Boards Manager from Tools > Board menu
  3. Type "esp32" in the search tool bar
  4. Select "Install" for esp32 platform


### Install serial drivers
These drivers allow for serial communication over the USB port of the Mac

1. Open: https://github.com/WCHSoftGroup/ch34xser_macos
2. Download: **CH34xVCPDriver.pkg**
3. Follow installation notes here: https://github.com/WCHSoftGroup/ch34xser_macos#installation

  **Note:** This will require admin privileges on the Mac.  No restart required on MacOS 12+

### Setup hardware
Connect peripherals to the LilyGO board

* Connect Antenna and GPS to LilyGO
<pic>
* Connect LILYGO to your computer using a USB-C cable.  LILYGO will draw power from the laptop to run.  You can use any USB source or battery to power the LILYGO once you have uploaded code to it.

### Arduino IDE configuration and dependencies
Initial setup of LilyGO board in the Arduino IDE

1. Connect LilyGO to your computer using a USB-C cable
2. Open the Arduino IDE
3. Go to the “Tools” menu
4. Select “Board:” > ESP32 Arduino > **ESP32 Wrover Module**
5. Back to Tools menu, ensure the following are chosen for each of the menus below:
  - Board:Upload Speed: 921600
  - Flash Frequency: 80MHz
  - Flash Mode: QIO
  - Partition Scheme: Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)
  - Core Debug Level: None
  - Port: /dev/cu.wchusbserialxxxxxxxxxxx

Congrats! At this point, you are ready to load sketches and test your hardware.

## Official LILYGO Github examples for board:
 https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G

## Helpful libraries and sample code for various objectives
Relevant Library Dependencies (optional), loaded via the Arduino Library Manager

* [TinyGSM](https://www.arduino.cc/reference/en/libraries/tinygsm/) : Communication libraries for LTE chipset.
* [ArduinoHTTPClient](https://www.arduino.cc/reference/en/libraries/arduinohttpclient/) : HTTP libraries for Arduino
* [Ethernet](https://www.arduino.cc/reference/en/libraries/ethernet/) : TCP/IP stack
* [StreamDebugger](https://www.arduino.cc/reference/en/libraries/streamdebugger/) : Various debugging tools
* [SSLClient](https://www.arduino.cc/reference/en/libraries/sslclient/) : SSL to enable HTTPS
* [IRremote](https://www.arduino.cc/reference/en/libraries/irremote/) : Libraries for sending information over IR

## Other Twilion Repos for the LilyGO
* Scott Lemon:
  * https://github.com/Wovyn/lilygo-t-sim7000g-asset-tracker-example
  * https://github.com/Wovyn/esp32-ble-advertisement-scanner-xiaomi-sensor-decoder
    * BLE Sensor Firmware: https://pvvx.github.io/ATC_MiThermometer/TelinkMiFlasher.html
* Brian Mgrdichian:
  * [Dual Core IR Receiver for ESP32](/examples/dual_core_esp32_IR/)
* Alex Goldman: TBA
