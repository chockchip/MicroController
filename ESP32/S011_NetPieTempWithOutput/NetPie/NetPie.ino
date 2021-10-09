
#include<Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include<Adafruit_AM2315.h>

// AM2351 for temp&humid sensor
Adafruit_AM2315 am2315;

// Wifi parameter
char ssid[] = "Thanis_2.4G";
char pass[] = "0895356743";
int baudRate = 115200;

// LED output pin
int LED = 15;

// NETPIE
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_client = "9c6463c2-1fde-45e3-8cb4-8bc2df4c5f05"; // ClientId
const char* mqtt_username = "Yb18A5kcJtj1Vu58wxh82Y8999J6JbNH";   // Token
const char* mqtt_password = "mVUhZdNKoHz9wA3Jxtrk#wYPAfaQ_dtI";   // Password

// Setup Mqtt
WiFiClient espClient;
PubSubClient client(espClient);

// Variable for time comparation
long lastMsg = 0;
int value = 0;

// Variable for send data to Mqtt
char msg[500];

// Function for connect to Netpie via Mqtt
void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection…");
        if (client.connect(mqtt_client, mqtt_username, mqtt_password)) {
            Serial.println("connected");
            client.subscribe("@msg/led"); //Topic that device subscribe
        } 
        else {
            // Case connect to the Netpie fail
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println("try again in 5 seconds");
            delay(5000);
        }
    }
}

// Function for recieve msg from Mqtt base on topic that subscribe
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    String message;
    for (int i = 0; i < length; i++) {
        message = message + (char)payload[i];
    }
    Serial.println(message);
    if(String(topic) == "@msg/led") {
        if(message == "on"){
            digitalWrite(LED,1);
            client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"on\"}}");
            Serial.println("LED on");
        }
        else if (message == "off"){
            digitalWrite(LED,0);
            client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"off\"}}");
            Serial.println("LED off");
        }
    }
}

void setup() {

  // Setup serial port
  Wire.begin();
  Serial.begin(baudRate);

  if (!am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
    while (1);
  }

  // Setup LED pin
  pinMode(LED, OUTPUT);

  // Setyp Wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
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
  client.setCallback(callback);
  Serial.println("Complete setup callback: ");
}

void loop() {
  float temperature, humidity;
  String place = "Location101";

  // Confirm connected to NETPIE
  if(!client.connected()){
    reconnect();
  }

  client.loop();

  long now = millis();
  if (now - lastMsg > 2000){
    lastMsg = now;
    ++value;
  }

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

  /*
  if(test == 1){
     client.publish("@msg/led", "on");
     test = 0;
  }
  else{
     client.publish("@msg/led", "off");
     test = 1;
  }
  Serial.println(test);
  */
  
  delay(2000);
}
