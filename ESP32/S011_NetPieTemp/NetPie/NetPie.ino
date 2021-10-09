
#include<Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include<Adafruit_AM2315.h>

// AM2351 for temp&humid sensor
Adafruit_AM2315 am2315;

// Wifi credential
char ssid[] = "Thanis_2.4G";
char pass[] = "0895356743";

// NETPIE
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_client = "9c6463c2-1fde-45e3-8cb4-8bc2df4c5f05"; // ClientId
const char* mqtt_username = "Yb18A5kcJtj1Vu58wxh82Y8999J6JbNH";   // Token
const char* mqtt_password = "mVUhZdNKoHz9wA3Jxtrk#wYPAfaQ_dtI";   // Password

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
int value = 0;
char msg[100];

void reconnect(){
  Serial.print("Attempting MQTT connection...");

  /* Start connect to Netpie via Mqtt protocal*/
  if(client.connect(mqtt_client, mqtt_username, mqtt_password)){
    Serial.println("connected");
    client.subscribe("@msg/led");
  }
  else{
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println("try again in 5 seconds");
    delay(5000);
  }
}

void setup() {

  /* Setup serial port */
  int baudRate = 115200
  
  Wire.begin();
  Serial.begin(baudRate);

  if (! am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
    while (1);
  }
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Setup Wifi */
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  
}

void loop() {
  float temperature, humidity;
  String place = "IdeoMix101";

  // Confirm connected to NETPIE
  if(!client.connected()){
    reconnect();
  }

  /* Function for keep the connection*/
  client.loop();

  // Start read temp and humidity
  if (!am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
   Serial.println("Failed to read data from AM2315");
   delay(1000);
   return;
  }

  String data = "{\"data\": {\"temperature\":" + String(temperature) +
                ",\"humidity\":" + String(humidity) + ",\"place\":\"" +
                String(place) + "\"}}";

  Serial.println(data);
  data.toCharArray(msg, (data.length()+1));
  client.publish("@shadow/data/update", msg);

  /**/

  /*
  long now = millis();
  if (now - lastMsg > 2000){
    lastMsg = now;
    ++value;
    client.publish("@msg/test", "Hello NETPIE 2020");
    Serial.println("Hello NETPIE 2020");
  }
  */

  
  delay(2000);
}
