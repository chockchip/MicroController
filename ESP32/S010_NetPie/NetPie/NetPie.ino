
#include <WiFi.h>
#include <PubSubClient.h>

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

void reconnect(){
  Serial.print("Attempting MQTT connection...");
  if(client.connect(mqtt_client, mqtt_username, mqtt_password)){
    Serial.println("connected");
  }
  else{
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println("try again in 5 seconds");
    delay(5000);
  }
}

void setup() {
  Serial.begin(115200);
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
}

void loop() {
  if(!client.connected()){
    reconnect();
  }

  client.loop();

  long now = millis();
  if (now - lastMsg > 2000){
    lastMsg = now;
    ++value;
    client.publish("@msg/test", "Hello NETPIE 2020");
    Serial.println("Hello NETPIE 2020");
  }
  delay(1000);
}
