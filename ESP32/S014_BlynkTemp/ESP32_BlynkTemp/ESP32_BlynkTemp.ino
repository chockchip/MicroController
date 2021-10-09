#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include<Wire.h>
#include <BlynkSimpleEsp32.h>
#include<Adafruit_AM2315.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Vv99SRQoPeMziKcTT_jKcJywfutl0wCf";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "pascal_2G";
char pass[] = "25041309";
int count_fail = 0;

Adafruit_AM2315 am2315;

void setup()
{
  Wire.begin();
  
  // Debug console
  Wire.setClock(400000);
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Blynk.begin(auth, ssid, pass,"blynk.iot-cm.com", 8080);

  Serial.println("ArduinoNa AM2315 Test!");

   if (! am2315.begin()) {
   /**
     * ถ้าขึ้นข้อความด้านล่าง ให้ต่อตัวต้านทาน 10k ตามรูปในหน้าสินค้าร้าน www.arduinona.com
     */
   Serial.println("Sensor not found, check wiring & pullups!");
   //while (1);
 }

 Serial.println("Ready to fun");
}

void loop()
{
  Blynk.run();

  float temperature, humidity;

   if (!am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
   Serial.println("Failed to read data from AM2315");
   count_fail = count_fail + 1;
   Blynk.virtualWrite(V2, count_fail);
   delay(2000);
   return;
  }
 
 Serial.print("Temp *C: "); Serial.print(temperature);
 Serial.print("\tHum %: "); Serial.println(humidity);


 Blynk.virtualWrite(V0, temperature);
 Blynk.virtualWrite(V1, humidity);
 
delay(2000);
}
