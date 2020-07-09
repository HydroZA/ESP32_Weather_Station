# ESP32 + DHT22 + BMP280 Weather Station
Code for the creation of a weather station using an ESP32, DHT22 and BMP280.

The ESP32 captures data from the sensors then uploads it to a web server using HTTP POST, and is then pushed to a MySQL database using PHP. The data is viewable over the internet in the format of graphs created using FusionXT as well as raw data in a PHP table format.

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
