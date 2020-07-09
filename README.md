# ESP32 + DHT22 + BMP280 Weather Station
Code for the creation of a weather station using an ESP32, DHT22 and BMP280.

The ESP32 captures data from the sensors then uploads it to a web server using HTTP POST, and is then pushed to a MySQL database using PHP. The data is viewable over the internet in the format of graphs created using FusionXT as well as raw data in a PHP table format. A running example is available [here](https://yahmes.pro/weather.php). 

[Imgur Album](https://imgur.com/a/PLyBMLo)

![img1](https://i.imgur.com/1JCntt6.jpg)

## Parts Required
### Bare Minimum
- [Heltec Wifi Kit 32](https://heltec.org/project/wifi-kit-32/)
- [DHT22](https://www.adafruit.com/product/385)
- [BMP280](https://www.adafruit.com/product/2651)
- 10k Ohm Resistor
- 27k Ohm Resistor
- 100k Ohm Resistor
- Generic Breadboard/perfboard and jumper wires

### Solar Power
- 18650 Lithium Battery
- 18650 Battery Holder
- [TP4056 Lithium Battery Charger Module](https://www.amazon.co.uk/TP4056-Lithium-Charging-Protection-Function/dp/B07BSVS842)
- [2x Mini Solar Panel (5/6V 1.2W)](https://makeradvisor.com/tools/mini-5v-solar-panel/)
- Box/Housing to protect from rain, etc

## Instructions
### Server Configuration
1. Install the LAMP stack on any compatible server. This can be local or over the internet, I chose to do it over the internet.
2. Create a new MySQL user with the username "esp_board" with full privileges
3. Create a new MySQL Database on your server called "esp_data"
4. Create the tables using [esp_data.sql](esp_data.sql)
5. Copy all the files in **/PHP/** to your Apache2 document root (Usually /var/www/html)

### Hardware Configuration
1. Wire the ESP32, sensors and optional solar power circuits
2. Modify Code to fit your needs. Eg. Server hostname, database password, pin numbers 
3. Connect to your PC and flash the [Weather_Station.ino](https://github.com/HydroZA/ESP32_Weather_Station/blob/master/ESP32%20Code/Weather_Station.ino) sketch to it
4. Open Serial monitor and verify all output of Temperature, Humidity and Pressure data

The ESP32 should now be pushing data to your web server and you should be able to access the data over local network or internet by visiting the **weather.php** page on your server using a web browser.
