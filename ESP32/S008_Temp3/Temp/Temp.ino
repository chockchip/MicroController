#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include<Wire.h>
#include <BlynkSimpleEsp32.h>
#include<Adafruit_AM2315.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "ogLbJPHf7GbG9fFUQMMwsh09Z5w7eun-";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "pp9896";
char pass[] = "airbus_a380";

Adafruit_AM2315 am2315;

int count_fail = 0;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
 }

 Blynk.begin(auth, ssid, pass, "192.168.1.37", 8080);
 
 Serial.println("ArduinoNa AM2315 Test!");

 if (! am2315.begin()) {
   /**
     * ถ้าขึ้นข้อความด้านล่าง ให้ต่อตัวต้านทาน 10k ตามรูปในหน้าสินค้าร้าน www.arduinona.com
     */
   Serial.println("Sensor not found, check wiring & pullups!");
   while (1);
 }
}

void loop() {
  
 float temperature, humidity;
Blynk.run(); 
 
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
