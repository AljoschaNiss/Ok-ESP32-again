#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

// #include "SD_MMC.h"

//adafruit platformio library lib_dev problem solved. But why?
// https://community.platformio.org/t/adafruit-dht22-sensor-library-problem/19459/3

//platformio hat eine eigen registry. In der Art wie npm ein für Node.js ist.


// build a serial monitor that is accessable via WiFi.
// https://randomnerdtutorials.com/esp32-webserial-library/
//This wasy you can see debugging notifactions about your running ESP32 via WiFi in the Browser.

#define DHTPIN      33
#define DHTTYPE     DHT22

#define SD_MISO     19
#define SD_MOSI     23
#define SD_SCLK     18
#define SD_CS_PIN   5

#define LED_PIN     25

String output_state_led = "off";

const char* ssid = "FRITZ!Box 7690 RL";
const char* password = "46023747369837710668";

WiFiServer server(80);

DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  dht.begin();

  // pinMode(LED_PIN, OUTPUT);

  // SD.begin(SD_CS);^

  // Initialize SD card
  // SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS_PIN);
  delay(500);

  //wait fpr serial port to connect.
  while (!Serial) { ; }

  if(!SD.begin(SD_CS_PIN)) {
    Serial.println("Card Mount Failed");
    return;
  } else {
    uint32_t cardSize = SD.cardSize() / (1024 * 1024);
    String str = "SDCard Size: " + String(cardSize) + "MB";
    Serial.println(str);
  }

  // Tipps and robotik tutorials: https://www.youtube.com/watch?v=PQhQfww-qGQ

  // Serial.println("initialization failed!");

  // uint8_t cardType = SD.cardType();
  // if(cardType == CARD_NONE) {
  //   Serial.println("No SD card attached");
  //   return;
  // }
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  if(h >= 59.0f && output_state_led == "off") {
    digitalWrite(LED_PIN, HIGH);
    output_state_led = "on";
  } else {
    digitalWrite(LED_PIN, LOW);
    output_state_led = "off";
  }
}



// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }