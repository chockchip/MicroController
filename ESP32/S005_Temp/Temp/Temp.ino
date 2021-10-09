#include<Wire.h>
#include<Adafruit_AM2315.h>

Adafruit_AM2315 am2315;

void setup() {
  
  /* 
  Setup the serial port
  buard rate: rate to transfer data
  */
  int baudRate = 115200;
  
  Wire.begin();
  Serial.begin(baudRate);
  while (!Serial) {
    delay(10);
 }
 
 Serial.println("ArduinoNa AM2315 Test!");

 if (!am2315.begin()) {
   /**
     * ถ้าขึ้นข้อความด้านล่าง ให้ต่อตัวต้านทาน 10k ตามรูปในหน้าสินค้าร้าน www.arduinona.com
     */
   Serial.println("Sensor not found, check wiring & pullups!");
   while (1);
 }
}

void loop() {
 float temperature, humidity;
 
 if (!am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
   /*Serial.println("Failed to read data from AM2315");
   delay(2000);*/
   return;
 }
 else{
  Serial.print("Temp *C: "); Serial.print(temperature);
  Serial.print("\tHum %: "); Serial.println(humidity);
  delay(10000);
 }

}
