#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <U8g2lib.h>  
#include <driver/adc.h>
#include <Adafruit_BMP280.h> 

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

const char* ssid     = "xxxx";
const char* password = "xxxxxx";

// Change to match your server hostname/IP
const char* serverNameTemp = "https://xxxxx/php/post-weather-data.php";
const char* serverNameBatt = "https://xxxxx/php/post-batt-data.php";

String apiKeyValue = "xxxxx";

// Init OLED
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 16, 15, 4);
char chBuffer[128];
#define FONT_ONE_HEIGHT 8  // font one height in pixels
#define FONT_TWO_HEIGHT 20 

String sensorName = "DHT22";
String sensorLocation = "Outside";

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1) delay(10);
  }

    /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  bmp_pressure->printSensorDetails();
  
  // OLED
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  
  dht.begin();
}

void loop() {
  // set deep sleep timer
  esp_sleep_enable_timer_wakeup(300000000);
  
  // configure voltage reading for battery
  pinMode(36, INPUT);
  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_11db);
  float v = adc1_get_voltage(ADC1_CHANNEL_0);

  // Get battery data
  String voltage = String (map(v, 0.0f, 4200.0f, 0.0f, 4.2f));
  String percentage = String(map(v, 0.0f, 4200.0f, 0, 100));
  
  u8g2.clear();
  sprintf(chBuffer, "%s", "Connecting To: ");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 0, chBuffer);
  sprintf(chBuffer, "%s", ssid);
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 31 - (FONT_ONE_HEIGHT / 2), chBuffer);
  u8g2.sendBuffer();  

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("not connected to wifi");
    delay(500);
  }
  
  sprintf(chBuffer, "Connected!");
  u8g2.clear();
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 0, chBuffer);
  u8g2.sendBuffer();
  
  HTTPClient http;
  HTTPClient http2;

  sensors_event_t pressure_event;
  bmp_pressure->getEvent(&pressure_event);

  // Get weather data and convert to String
  String temp = String(dht.readTemperature());
  String humidity = String (dht.readHumidity());
  String pressure = String (pressure_event.pressure) + "hPa";

  Serial.println(temp);
  Serial.println(humidity);
  Serial.println(pressure);
  
  String httpRequestTempData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                          + "&location=" + sensorLocation + "&temperature=" + temp
                          + "&humidity=" + humidity + "&pressure=" + pressure + "";
                          
  String httpRequestBattData = "api_key=" + apiKeyValue + "&voltage=" + voltage + "&percentage=" + percentage + "";

  // Display weather data on OLED
  sprintf(chBuffer, "Latest Readings:");
  u8g2.drawStr(0, FONT_ONE_HEIGHT, chBuffer);

  u8g2.setFont(u8g2_font_6x10_tr);
  sprintf(chBuffer, "Temperature: %s", temp);
  u8g2.drawStr(0, (FONT_TWO_HEIGHT * 2), chBuffer);
  sprintf(chBuffer, "Humidity: %s", humidity);
  u8g2.drawStr(0, (FONT_TWO_HEIGHT * 2.5), chBuffer);
  u8g2.sendBuffer();
  
  if (WiFi.status() == WL_CONNECTED)
  {
    // POST Weather Data
    http.begin(serverNameTemp);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.POST(httpRequestTempData);
    http.end();

    //POST Battery Data
    http2.begin(serverNameBatt);
    http2.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http2.POST(httpRequestBattData);
    http2.end();

    // Disconnect wifi and deep sleep
    WiFi.disconnect();
    esp_deep_sleep_start();
  }
}
