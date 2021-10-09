#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include<Wire.h>
#include <BlynkSimpleEsp32.h>
#include<Adafruit_AM2315.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "cJMm0qI--HXqJHvYRnZ2ywvuVN-hRZPy";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "pp9896";
char pass[] = "airbus_a380";

void setup() {

  Serial.begin(115200);
  while (!Serial) {
    delay(10);
 }

 Blynk.begin(auth, ssid, pass, "192.168.1.37", 8080);
 
}

void loop() {

Blynk.run(); 
 

}
