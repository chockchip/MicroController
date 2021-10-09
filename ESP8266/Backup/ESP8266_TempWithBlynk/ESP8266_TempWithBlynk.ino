#include "AM2315_I2C.h"

#include "WiFi.h"
#include "WiFiClient.h"
#include "Wire.h"

// Define ESP8266 Pin
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;

//ESP8266 I2C Pins
const uint8_t dataPin = D1;
const uint8_t clockPin = D2;

AM2315_I2C am2315;

void setup() {
  Serial.begin(115200);

  Wire.pins(dataPin, clockPin);

  delay(100);

  Serial.println();

  am2315.begin(dataPin, clockPin);

  if (!am2315.dataReady)
    Serial.println("AM2315 sensor was not found");
  else
    Serial.println("AM2315 sensor is ready");
}

void loop() {

  //Non-blocking data reading
  am2315.acquireData();

  if (am2315.dataReady) {

    Serial.print("Humidity: ");
    Serial.println(am2315.humidity());
    Serial.print("Temperature: ");
    Serial.println(am2315.temperature());
    //Serial.println();

  }

}
