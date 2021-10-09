#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Vv99SRQoPeMziKcTT_jKcJywfutl0wCf";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "pascal_2G";
char pass[] = "25041309";

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass,"blynk.iot-cm.com", 8080);
}

void loop()
{
  Blynk.run();
}
