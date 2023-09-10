# RF433 to WiFi
This project is a prototype to command some home devices via RF433 from the internet.

Sources and ideas:
* ESP32 RMT peripheral with 315 MHz or 433 MHz EV1527, HS1572, PT2240 and similar https://www.espboards.dev/blog/esp32-433-rmt-ev1527/
* https://github.com/sui77/rc-switch/wiki
* https://www.instructables.com/RF-315433-MHz-Transmitter-receiver-Module-and-Ardu/
* https://www.youtube.com/watch?v=KA_YE7AvFn0
* https://www.instructables.com/Using-an-ESP8266-to-Control-Mains-Sockets-Using-43/
* https://www.liwen.id.au/arduino-rf-codes/
* https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
* https://github.com/ninjablocks/433Utils/tree/master/RPi_utils
* https://lastminuteengineers.com/433mhz-rf-wireless-arduino-tutorial
* https://github.com/sui77/rc-switch
* https://www.espboards.dev/blog/esp32-433-rmt-ev1527/


## Background info
### RF433
### RF433 ASK
### RF433 HW devices
The TX/RX devices are quite cheap and usually come in a bundle of 2. I've used the receiver XLC-RF-5V and the sender XKFST, as in the picture !(HW_devices_RX_TX.jpg).
### Antennas
A 433MHz wave has a wavelength of about 69cm. This is calculated from wavelenght = c / freq. You may try to use a simple wire straight up which is a standard monopole antenna. The best lenght to achieve some resonant will be wavelenght/4. So, you may use a sinlge wire of 17.3cm. I've tried the "433MHz air cooled antenna" but I haven't confirmed any improvements. It may be that I haven't used the right cable width or something else, so I reverted to the basic monopole. The reception of my own transmitter seems to do not go above 1m, but I managed to control some devices at around 10m. So am still not sure whats going on.
### Arduino HW devices
I've used an ESP8266.
### Connecting
Quite simply power the devices as described in the components. I've used the 3.3v successfully but also the Vin. (5V). The data imput can be connected to where you configure the ESP. In this case:
* RX - D2
* TX - D3


## VERSION 1 
* Objective:
* Connected devices: mmm
