#include<WiFi.h>
#include<FirebaseESP32.h>

#define WIFI_STA_NAME "pp9896"
#define WIFI_STA_PASS "airbus_a380"
#define LED_BUILTIN 2
#define FIREBASE_HOST "https://plant-demo-db733-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "qBBWC8Rt5FuWp3bKgVIwQvaVHWHXWiIbds5btZpC"

FirebaseData firebaseData;
FirebaseJson json;
int Vresistor = A0;
int Vrdata = 0;
int i = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Firebase.setReadTimeout(firebaseData, 1000 * 60);

  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  Serial.println("------------------------------------");
  Serial.println("Connected...");
}

void loop() 
{ 
  int Sdata = 100;
  if(i < 100){
    i += 1;
  }
  else{
    i = 0;
  }
  Serial.println(i);
  delay(1000);
  json.set("/data", i);
  Firebase.updateNode(firebaseData,"/sensor",json);  
}
