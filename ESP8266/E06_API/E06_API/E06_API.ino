#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Thanis_2.4G";
const char* password = "0895356743";

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting...");
  }
}

void loop() 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http; //Object of class HTTPClient
    http.begin("http://jsonplaceholder.typicode.com/users/1");
    int httpCode = http.GET();

    if (httpCode > 0) 
    {
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonDocument jsonBuffer(bufferSize);
      //JsonObject& root = jsonBuffer.parseObject(http.getString());

      deserializeJson(jsonBuffer, http.getString());
      int id = jsonBuffer["id"]; 
      const char* name = jsonBuffer["name"]; 
      const char* username = jsonBuffer["username"]; 
      const char* email = jsonBuffer["email"]; 

      Serial.print("Name:");
      Serial.println(name);
      Serial.print("Username:");
      Serial.println(username);
      Serial.print("Email:");
      Serial.println(email);
    }
    http.end(); //Close connection
  }
  delay(60000);
}
